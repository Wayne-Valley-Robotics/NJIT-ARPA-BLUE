#include "drive_interface.h"
namespace drive_interface
{

    /*
    encoder pin A must be capable of interrupts.

    there are six interrupt pins on the arduino mega: 2, 3, 18, 19, 20, 21
    pins 20 & 21 are not available to use for interrupts while they are in use for I2C (we aren't using them for that at the moment)
    most notably though they also have **external pull-ups that cannot be disabled**

    only encoderPinA needs to be an interrupt pin!
    */

    // workaround for interrupts not being able to call local functions
    // void m1EncoderISR();
    // void m2EncoderISR();
    // void m3EncoderISR();
    // void m4EncoderISR();

    // pin, invert, encoderPinA, encoderPinB
    // only the first two are required
    // MOTOR m1(9, 8, 0, 2, 3, &m1EncoderISR);
    // void m1EncoderISR() { m1.readEncoder(); };

    // MOTOR m2(10, 9, 0, 18, 19, &m2EncoderISR);
    // void m2EncoderISR() { m2.readEncoder(); };

    // MOTOR m3(11, 10, 0, 20, 21, &m3EncoderISR);
    // void m3EncoderISR() { m3.readEncoder(); };

    // MOTOR m4(12, 11, 0, 20, 21, &m4EncoderISR);
    // void m4EncoderISR() { m4.readEncoder(); };

    // Which motors are which?
    MOTOR m1(5, 26, true);
    MOTOR m2(8, 31); // inverted
    MOTOR m3(6, 29);

    void initMotors() // abstracted to drive_interface to avoid depending on this header in main.cpp
    {
        m1.begin();
        m2.begin();
        m3.begin();
        // m4.begin();
    }

    /*****************************/
    /*   I N P U T   L O G I C   */
    /*****************************/

    // generic tri drive controls
    void triDrive(int joyLY, int joyLX, int joyRX)
    {
        // slater's magic v2
        int m1Val = (-joyLX * -1 * .5) + (joyLY * sqrt(3) / 2) + joyRX; // (sqrt(3))/2 = .866
        int m2Val = (-joyLX * -1 * .5) - (joyLY * sqrt(3) / 2) + joyRX;
        int m3Val = -joyLX + joyRX;

        m1.setPower(m3Val);
        m2.setPower(m2Val);
        m3.setPower(m1Val);
    }
}
