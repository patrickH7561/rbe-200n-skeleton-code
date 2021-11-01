#include <PIDcontroller.h>

class WallFollowingController
{
public:
    float leftEffort = 0;
    float rightEffort = 0;
    float Circumfrence = 19.792033717616;
    float BaseSpeed = 25;
    float Speed5 = 15;
    
protected:
    float targetDistance = 30;

    PIDController piWallFollow;

public:
    WallFollowingController(void) : piWallFollow(.23, 0, .01) {} 
    void processDistanceReading(float distance);
    void handleKeyPress(int16_t key);
};