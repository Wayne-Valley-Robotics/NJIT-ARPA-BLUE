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
        if (_vertP != vertP)
        {
            _vertP = vertP;
            mV.setPower(vertP);
        }
        if (_horizP != horizP)
        {
            _horizP = horizP;
            mH.setPower(horizP);
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
        if (!digitalRead(VERT_LIMIT_SWITCH_PIN_TOP))
            vertP += speed;
    }

    void moveDown(int speed)
    {
        if (!digitalRead(VERT_LIMIT_SWITCH_PIN_BOTTOM))
            vertP -= speed;
    }

    void moveLeft(int speed)
    {
        // if (digitalRead(HORIZ_LIMIT_SWITCH_PIN_L))
            horizP -= speed;
    }

    void moveRight(int speed)
    {
        if (!digitalRead(HORIZ_LIMIT_SWITCH_PIN_R))
            horizP += speed;
    }
}