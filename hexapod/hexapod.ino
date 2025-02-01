#include "Hexapod.h"

SharedData robot;
Hexapod  hexapod;

HardwareSerial hSerial(2);

#define RXD2 4
#define TXD2 23

const int t = 2000;

void setup()
{
  Serial.begin(9600);
  hexapod.init(ROBOT_USER_MODE);
  robot.setLed(WHITE);
  robot.setHeight(75);

  hSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
}


void loop()
{
  if (hSerial.available())
  {
    String a = hSerial.readStringUntil('\n');
    a.trim();

    if ( a == "C")
    {
      robot.rotate(RIGHT, t);
    }
    else if (a == "A")
    {
      robot.rotate(LEFT, t);
    }
    else if (a == "F")
    {
      robot.move(FORWARD, t);
    }
    else if (a == "B")
    {
      robot.move(BACKWARD, t);
    }
    else if (a == "L")
    {
      robot.move(LEFT, t);
    }
    else if (a == "R")
    {
      robot.move(RIGHT, t);
    } 
    }
    delay(20);
}

void setLED(){
    robot.setLedStatic(0, RED);
    robot.setLedStatic(1, RED);
    robot.setLedStatic(2, RED);
    robot.setLedStatic(3, BLUE);
    robot.setLedStatic(4, BLUE);
    robot.setLedStatic(5, BLUE);
}
