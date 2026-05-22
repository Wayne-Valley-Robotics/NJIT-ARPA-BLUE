#pragma once
#include "motor.h"

namespace drive_interface
{
    void initMotors();
    void procDrive(int joyLY, int joyLX, int joyRX);
    
    void initClaw();
    void clawMechanism(bool circle, bool triangle, bool upBtn, bool downBtn);

}