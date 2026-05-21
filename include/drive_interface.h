#pragma once
#include "motor.h"

namespace drive_interface
{

    void initMotors();
    void triDrive(int joyLY, int joyLX, int joyRX);
}