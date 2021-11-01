#include <Arduino.h>
#include <RBE-200n-lib.h>
#include "robot.h"

Robot robot;

void setup() 
{
  Serial.begin(115200);
  delay(500);
  Serial.println("We are at least in the right file");
  robot.init();
}

void loop()
{
  robot.loop();
}