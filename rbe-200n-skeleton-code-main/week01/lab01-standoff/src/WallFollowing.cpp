#include "WallFollowing.h"
#include "RemoteConstants.h"

void WallFollowingController::processDistanceReading(float distance)
{
    float error = targetDistance - distance;
    float effort = piWallFollow.CalcEffort(error);
    Serial.println(effort);
    Serial.print(targetDistance);
    Serial.print('\t');
    Serial.print(distance);
    Serial.print('\t');
    Serial.print(error);
    Serial.print('\t');
    Serial.print(effort);
    Serial.print('\t');
    Serial.print(leftEffort);
    Serial.print('\t');
    Serial.print(rightEffort);
    Serial.print('\t');

    if (error > 0)
    {
        leftEffort = BaseSpeed;
        rightEffort = BaseSpeed + effort;
    }
    else if (error < 0)
    {
        leftEffort = BaseSpeed + effort;
        rightEffort = BaseSpeed;
    }
    else
    {
        leftEffort = BaseSpeed;
        rightEffort = BaseSpeed;
    }
}

void  WallFollowingController::handleKeyPress(int16_t key)
{
    //Serial.print("I think it might have worked but dont be to happy.");

    switch(key)
    {
        case remoteUp:
            BaseSpeed += Speed5;
            break;

        case remoteDown:
            BaseSpeed -= Speed5;
            break;

        default:
            if(key >= remote1 && key <= remote9)
            {
                targetDistance = key * 10;
            }
            break;
    }
}