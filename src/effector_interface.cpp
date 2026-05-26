#include "effector_interface.h"
#include "motor.h"

namespace effector_interface
{
    // probably only called once so cant hurt to be a constexpr right??
    constexpr int VERT_LIMIT_SWITCH_PIN_TOP = 37;
    constexpr int VERT_LIMIT_SWITCH_PIN_BOTTOM = 36;

    // constexpr int HORIZ_LIMIT_SWITCH_PIN_L = 0;
    constexpr int HORIZ_LIMIT_SWITCH_PIN_R = 38;

    int vertP = 0;
    int horizP = 0;

    MOTOR mH(10, 27, true);
    MOTOR mV(9, 30, true);

    void init()
    {
        mH.begin();
        mV.begin();

        pinMode(VERT_LIMIT_SWITCH_PIN_TOP, INPUT_PULLUP);
        pinMode(VERT_LIMIT_SWITCH_PIN_BOTTOM, INPUT_PULLUP);
        // pinMode(HORIZ_LIMIT_SWITCH_PIN_L, INPUT_PULLUP);
        pinMode(HORIZ_LIMIT_SWITCH_PIN_R, INPUT_PULLUP);
    }

    void proc()
    {
        static int _vertP = 0;
        static int _horizP = 0;

        // P has no real meaning tbh
        // lowk it means whatever the logic below uses it as
        // for now it means power but it could literally just mean target speed as well

        // halt at upper limit
        if (vertP > 0 && digitalRead(VERT_LIMIT_SWITCH_PIN_TOP))
        {
            vertP = 0;
        }
        // halt at lower limit
        if (vertP < 0 && digitalRead(VERT_LIMIT_SWITCH_PIN_BOTTOM))
        {
            vertP = 0;
        }

        // halt at horizontal limit
        if (horizP > 0 && digitalRead(HORIZ_LIMIT_SWITCH_PIN_R))
        {
            horizP = 0;
        }

        // there is no horiz left limit switch

        // set power if new inputs are available
        if (_vertP != vertP)
        {
            mV.setPower(vertP);
            _vertP = vertP;
        }
        if (_horizP != horizP)
        {
            mH.setPower(horizP);
            _horizP = horizP;
        }

        // reset variables so they dont continue in that direction forever.
        // that means inputs must be refreshed before proc() is called again
        _vertP = vertP;
        _horizP = horizP;
        vertP = 0;
        horizP = 0;
    }

    void moveUp(int speed)
    {
        vertP += speed;
    }

    void moveDown(int speed)
    {
        vertP -= speed;
    }

    void moveLeft(int speed)
    {
        horizP -= speed;
    }

    void moveRight(int speed)
    {
        horizP += speed;
    }
}