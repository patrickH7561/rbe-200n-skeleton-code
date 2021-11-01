#include "robot.h"
#include "RemoteConstants.h"
#include <rangefinder.h>
#include <IRdecoder.h>

Rangefinder RF;

Robot::Robot(void)
{
    //nothing to see here; move along...
}

void Robot::init(void)
{
    chassis.init();
    irDecoder.init(IR_PIN);
    RF.attach(16,17);
    Serial.println("Just trying to figure it out");
}

void Robot::loop() 
{
    //check the IR remote
    int16_t keyCode = irDecoder.getKeyCode();
    if(keyCode != -1) handleIRPress(keyCode);

    //check the distance sensor
    float distanceReading = 0;
    bool hasNewReading = RF.myGetDistance(distanceReading);
    if(hasNewReading) handleNewDistanceReading(distanceReading);
}

void Robot::handleIRPress(int16_t key)
{
    Serial.println(key);
    if(key == remoteStopMode)
    {
        chassis.stop();
        robotState = ROBOT_IDLE;
        return;
    }

    switch(robotState)
    {
        case ROBOT_IDLE:
            if(key == remoteLeft)
            {
                robotState = ROBOT_STANDOFF;
                Serial.println("Entered Standoff Mode");
            }
            if(key == remoteRight)
            {
                robotState = ROBOT_WALL_FOLLOWING;
                Serial.println("Entered Wallfollow Mode");
            }
            break;
        case ROBOT_STANDOFF:
            standoffController.handleKeyPress(key);
            break;
        case ROBOT_WALL_FOLLOWING:
            wallfollowingcontroller.handleKeyPress(key);
            break;
        default:
            break;
    }
}

void Robot::handleNewDistanceReading(float distanceReading)
{
    // //comment out after you verify this works
    //  Serial.println("Patrick you are being Stupid");
    //  Serial.print(millis());
    //  Serial.print('\t');
    //  Serial.print(distanceReading);
    //  Serial.print('\t');
    
    //TODO: Add wall following behaviour

    if(robotState == ROBOT_STANDOFF)
    {
        standoffController.processDistanceReading(distanceReading);
        chassis.setMotorEfforts(standoffController.leftEffort, standoffController.rightEffort);
    } 
     if(robotState == ROBOT_WALL_FOLLOWING)
    {
        wallfollowingcontroller.processDistanceReading(distanceReading);
        chassis.setMotorEfforts(wallfollowingcontroller.leftEffort, wallfollowingcontroller.rightEffort);
    }  

    Serial.print('\n');
}