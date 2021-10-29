#include <Arduino.h>
#include "RBE1001Lib.h"
#include "BlueMotor.h"
#include "CServo.h"
#include "IRdecoder.h"
#include "RemoteConstants.h"
#include "UltraSonic.h"
#include "Chassis.h"
#include "LineSensors.h"

BlueMotor BM;
Servo ContServo;
IRDecoder decoder(15);
uint16_t Code;
UltraSonic US;
Chassis Bot;
LineSensors LS;
Timer timerWrist(7500);

float kp = .01;
float kd = .008; 
float lasterror = 0;
float base = .15;

void setup() {
  Serial.begin(9600);
  BM.setup();
  ContServo.attach(33);
  decoder.init();
  US.rangefinder.attach(16,17);
  Bot.left.attach();
  Bot.right.attach();
  LS.rightLine.attach(39);
  LS.leftLine.attach(36);
}

void drivetoobject(int Dist){
  while (US.getDistance() > Dist)
  {
    int CurrentD = US.getDistance();
    Serial.println(US.getDistance());
    Bot.setDriveEffort(CurrentD * .01, CurrentD * .011);
  }
  Bot.setDriveEffort(0, 0);
}

void FollowTheDamnLine(){
  float error = LS.getError();
  float motorspeed = kp * error + kd * (error-lasterror);
  lasterror = error;
  float leftVal = base - motorspeed;
  float rightVal = base + motorspeed;
  // Serial.println(leftVal);
  // Serial.println(rightVal);
  Bot.setDriveEffort(leftVal, rightVal);
}

void turnWithLine(int direction)
{
  Bot.left.setSpeed(120 * direction);
  Bot.right.setSpeed(-120 * direction);
  delay(500);
  if (direction == 0)
  return;
  while (true){
    if (LS.driveSensorsOnLine())
    {
      delay(190);
      Bot.setDriveEffort(0,0);
      break;
    }
  }
}

void centerOnCrossSection(int direction)
{
  Bot.forward(8);
  //+1 is turn right
  //-1 is turn left
  //0 only centers does not turn
  turnWithLine(direction);
}

void depresso(){
  uint16_t code;
  while(!LS.driveSensorsOnLine()){
    FollowTheDamnLine();
  }
  centerOnCrossSection(-1);
  BM.moveTo(5500);//move arm to 45degree
    while(US.getDistance() > 15){
    FollowTheDamnLine();
  }
  drivetoobject(6);//drive to 45
  ContServo.write(0);//go down
  timerWrist.reset();
  while(!timerWrist.isExpired()){
  }
  ContServo.write(98);
  while(code!=remotePlayPause){
    code = decoder.getKeyCode();
  }
  BM.moveTo(6500); // get past screws 45
  turnWithLine(-1);
  while(!LS.driveSensorsOnLine()){
    FollowTheDamnLine();
  }
  centerOnCrossSection(-1);
  while(US.getDistance() > 8){
    FollowTheDamnLine();
  }

  BM.moveTo(0); // arm to platform
  ContServo.write(180);//go down
  timerWrist.reset();
  while(!timerWrist.isExpired()){

  }
  ContServo.write(98);

  while(!timerWrist.isExpired()){
    code = decoder.getKeyCode();
  }

  Bot.forward(-9);
  Bot.left.startMoveFor(-180, 180);
  Bot.right.moveFor(180, 180);

  while(!LS.driveSensorsOnLine()){
    FollowTheDamnLine();
  }

  centerOnCrossSection(1);




}

void loop() {
 Code = decoder.getKeyCode();
 if (Code == remote1)
 {
    Serial.println("Arm Down");
    BM.setEffort(255, true);
 }
else if (Code == remote2)
 {
    Serial.println("Arm Up");
    BM.setEffort(255, false);
 }
else if (Code == remote3)
 {
    Serial.println("Arm Stop");
    BM.setEffort(0, false);
 }
else if (Code == remote4)
 {
       ContServo.write(180);
 }
else if (Code == remote5)
{
    ContServo.write(0);
}
else if (Code == remote6)
{
    ContServo.write(98);
}
else if (Code == remote7)
{
    Serial.println("FOLLOW LINE");
  while(!LS.driveSensorsOnLine()){
    FollowTheDamnLine();
  }
  Serial.println("EXIT FOLLOW LINE");
  centerOnCrossSection(-1);
}
else if(Code == remote8)
{
    Serial.print(BM.getPosition());
}
else if(Code == remote9)
{
   Bot.setDriveEffort(0,0);
}
else if(Code == remotePlayPause)
{
   depresso();
}
}




