#include <Arduino.h>
#include <SPI.h>
#include <Chassis.h>
#include <Rangefinder.h>
#include <PIDcontroller.h>

Chassis Robot;
Rangefinder RF;
PIDController PID(10);
float MyDist = 0.0;
float TargetDist = 10; 
float BaseDrive = .5;

void setup() {
  delay(1000);
  Serial.begin(115200);
  Robot.init();
  RF.attach(16,17); 
}
void loop() {
  RF.myGetDistance(MyDist);
  float Effort = CalcEffort(MyDist);
  Serial.println(Effort);

  if (Effort > 0)
  {
    Robot.leftMotor.setEffort(BaseDrive + Effort);
    Robot.rightMotor.setEffort(Effort);
  }
  else if (Effort < 0)
  {
    Robot.rightMotor.setEffort(Effort);
    Robot.leftMotor.setEffort(BaseDrive + Effort);
  }
  else 
  {
    Robot.setMotorEfforts(Effort);
  }
}