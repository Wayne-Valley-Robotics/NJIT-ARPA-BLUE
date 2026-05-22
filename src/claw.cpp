#include "drive_interface.h"

namespace claw
{
    MOTOR claw(9, 33, true); //true flase wgho knows
    MOTOR vert(8, 31, true);


void initClaw()
{
    claw.begin();
    vert.begin();
}

void working()
{
    
}

}