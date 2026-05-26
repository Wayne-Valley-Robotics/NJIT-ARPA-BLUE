#pragma once
#include "motor.h"

namespace drive_interface
{

    void init();
    void triDrive(int joyLY, int joyLX, int joyRX);
}