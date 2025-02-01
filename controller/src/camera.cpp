#include <WiFiUdp.h>
#include <OV7670.h>
#include "config.h"

const int HEIGHT = 240;
const int WIDTH = 320;
const int PACKET_HEIGHT = 2;

OV7670 *camera;
WiFiUDP udp;

void setupCamera() {
    camera = new OV7670(OV7670::Mode::QVGA_RGB565, SIOD, SIOC, VSYNC, HREF, XCLK, PCLK, D0, D1, D2, D3, D4, D5, D6, D7);
    for(int i = 0; i < 20; i++) {
        camera->oneFrame();
    }
}

void udpTask(void *pvParameters) {
    while(1) {
        for(int y = 0; y < HEIGHT / PACKET_HEIGHT; y++) {
            udp.beginPacket(CLIENT_IP, CLIENT_PORT);
            udp.write(y);
            udp.write(&camera->frame[y * PACKET_HEIGHT * WIDTH * 2], WIDTH * PACKET_HEIGHT * 2);
            udp.endPacket();
            vTaskDelay(pdMS_TO_TICKS(600));
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
        camera->oneFrame();
    }
}