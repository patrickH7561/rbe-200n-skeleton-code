#include <Arduino.h>
#include <SPI.h>
#include <Chassis.h>
#include <Rangefinder.h>
#include <PIDcontroller.h>
#include <robot.h>

Rangefinder RF;
PIDController PID(10);
float MyDist = 0.0;
float TargetDist = 10; 
float BaseDrive = 10;

void setup() {
  delay(1000);
  Serial.begin(115200);
  chassis.init();
  RF.attach(16,17); 
  PID.SetKp(1);
  PID.SetKi(.05);
  PID.SetKd(.01);
}
void loop() {
  RF.myGetDistance(MyDist);
  float Effort = PID.CalcEffort(MyDist);
  if (Effort > 0)
  {
    chassis.setMotorEfforts(BaseDrive, Effort + BaseDrive);
  }
  else if (Effort < 0)
  {
    chassis.setMotorEfforts(Effort + BaseDrive, BaseDrive);
  }
  else 
  {
    chassis.setMotorEfforts(BaseDrive, BaseDrive);
  }
}

  