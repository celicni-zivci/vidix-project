#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "camera.h"
#include "robot.h"

void wifiSetup() {
    IPAddress localIP(192, 168, 1, 1); 
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.softAPConfig(localIP, gateway, subnet);
    WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);

    Serial.println("ESP32 AP Started!");
    Serial.print("AP IP Address: ");
    Serial.println(WiFi.softAPIP());
}

void setup() {
    Serial.begin(115200);

    wifiSetup();
    setupCamera();
    setupRobot();

    xTaskCreate(udpTask, "UDP", 4096, NULL, 1, NULL);
    xTaskCreate(tcpTask, "TCP", 4096, NULL, 1, NULL);
}

void loop() {

}