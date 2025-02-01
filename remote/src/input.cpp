#include <WiFi.h>
#include "input.h"
#include "config.h"

const int PIN_LR = 34;
const int PIN_UD = 35;
const int PIN_A = 32;
const int PIN_B = 33;

const char LEFT = 'L';
const char RIGHT = 'R';
const char FORWARD = 'F';
const char BACK = 'B';
const char CLOCKWISE = 'C';
const char ANTICLOCKWISE = 'A';

xQueueHandle inputQueue;
WiFiClient client;

void setupInput() {
    pinMode(PIN_A, INPUT_PULLUP);
    pinMode(PIN_B, INPUT_PULLUP);
    inputQueue = xQueueCreate(20, 1);
}

void tcpTask(void *pvParameters) {
    char button;

    while (1) {
        if (!client.connected()) {
            Serial.println("Connecting to server...");
            if (client.connect(SERVER_IP, SERVER_PORT)) {
                Serial.println("Connected to server.");
            } else {
                Serial.println("Connection failed.");
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }
        }

        if(xQueueReceive(inputQueue, &button, pdMS_TO_TICKS(300))) {
            client.println(button);
            Serial.printf("Sent: %c\n", button);
        }

        if (client.available()) {
            String response = client.readStringUntil('\n');
            Serial.print("Server response: ");
            Serial.println(response);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }    
}

void inputTask(void *pvParameters) {
    while(1) {
        int buttonLR = analogRead(PIN_LR);
        int buttonUD = analogRead(PIN_UD);
        int buttonA = digitalRead(PIN_A);
        int buttonB = digitalRead(PIN_B);

        if(buttonLR > 4000) {
            xQueueSend(inputQueue, &LEFT, 500);
        } else if(buttonLR > 1900 && buttonLR < 2000) {
            xQueueSend(inputQueue, &RIGHT, 500);
        } else if(buttonUD > 4000) {
            xQueueSend(inputQueue, &FORWARD, 500);
        } else if(buttonUD > 1900 && buttonUD < 2000) {
            xQueueSend(inputQueue, &BACK, 500);
        }

        if(buttonA == LOW) {
            xQueueSend(inputQueue, &CLOCKWISE, 500);
        }
        if(buttonB == LOW) {
            xQueueSend(inputQueue, &ANTICLOCKWISE, 500);
        }

        vTaskDelay(pdMS_TO_TICKS(300));
    }
}