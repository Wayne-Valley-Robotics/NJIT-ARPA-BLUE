#include "drive_interface.h"

namespace claw
{
    MOTOR clawMotor(9, 33, true); //true flase wgho knows
    MOTOR verticalMotor(8, 31, true);


void initClaw()
{
    clawMotor.begin();
    verticalMotor.begin();
}

void clawMechanism(bool circle, bool triangle, bool upBtn, bool downBtn)
{
    int clawSpeed = 0;
    if(circle) // open claw
    {
        clawSpeed = 200;
    }
    else if(triangle) //close claw
    {
        clawSpeed = -200;
    }

    int vertSpeed = 0;
    if(upBtn)
    {
        vertSpeed = 130;
    }
    else if(downBtn)
    {
        vertSpeed = -130;
    }

    clawMotor.setPower(clawSpeed);
    verticalMotor.setPower(vertSpeed);

}

}