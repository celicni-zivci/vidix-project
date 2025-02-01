#include <WiFi.h>
#include "config.h"
#include "robot.h"

WiFiServer server(TCP_PORT);
HardwareSerial hSerial(2);

void tcpTask(void *pvParameters) {
    while(1) {
        WiFiClient client = server.available();
        if (client) {
            Serial.println("Client connected.");
            while (client.connected()) {
                if (client.available()) {
                    String data = client.readStringUntil('\n');
                    Serial.print("Received: ");
                    Serial.println(data);

                    hSerial.println(data);
                }
                vTaskDelay(pdMS_TO_TICKS(100));
            }
            client.stop();
            Serial.println("Client disconnected.");
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void setupRobot() {
    hSerial.begin(9600, SERIAL_8N1, RXD, TXD);
    server.begin();
}