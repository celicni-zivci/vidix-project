#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>
#include <WiFiUdp.h>
#include "display.h"
#include "config.h"

const int HEIGHT = 240;
const int WIDTH = 320;
const int PACKET_HEIGHT = 2;

Adafruit_ILI9341 tft = Adafruit_ILI9341(5, 21);
uint8_t frame[WIDTH * PACKET_HEIGHT * 2 + 1];

WiFiUDP udp;

void setupDisplay() {
    tft.begin();
    tft.fillScreen(ILI9341_BLACK);
    tft.setRotation(3);
    udp.begin(UDP_PORT);
}

void udpTask(void *pvParameters) {
    while(1) {
        int packetSize = udp.parsePacket();
        if(packetSize) {
            int len = udp.read(&frame[0], sizeof(frame));
            if(len > 0) {
                tft.drawRGBBitmap(0, (uint16_t)frame[0] * 2, (uint16_t*)&frame[1], WIDTH, PACKET_HEIGHT);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
   } 
}