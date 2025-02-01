#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "input.h"
#include "display.h"

void setup(){
    Serial.begin(9600);

    IPAddress staticIP(192, 168, 1, 100);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.config(staticIP, gateway, subnet);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to AP...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nConnected!");
    Serial.print("Station IP Address: ");
    Serial.println(WiFi.localIP());
    
    setupInput();
    setupDisplay();

    xTaskCreate(inputTask, "input", 4096, NULL, 1, NULL);
    xTaskCreate(udpTask, "UDP", 8192, NULL, 1, NULL);
    xTaskCreate(tcpTask, "TCP", 8192, NULL, 1, NULL);
}

void loop() {

}