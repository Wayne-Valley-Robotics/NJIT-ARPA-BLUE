#pragma once
#include "motor.h"

namespace drive_interface
{

    void initMotors();
    void procDrive(int joyLY, int joyLX, int joyRX);
}