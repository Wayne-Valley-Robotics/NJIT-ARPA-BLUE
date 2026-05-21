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
    void mecanumFrontL_EncoderISR();
    void mecanumFrontR_EncoderISR();
    void mecanumRearL_EncoderISR();
    void mecanumRearR_EncoderISR();

    // pwm, dir, invert, encoderPinA, encoderPinB
    // only the first two are required
    MOTOR mecanumFrontL(4, 23, false, 2, 22, &mecanumFrontL_EncoderISR);
    void mecanumFrontL_EncoderISR() { mecanumFrontL.readEncoder(); };

    MOTOR mecanumFrontR(5, 25, true, 3, 24, &mecanumFrontR_EncoderISR);
    void mecanumFrontR_EncoderISR() { mecanumFrontR.readEncoder(); };

    MOTOR mecanumRearL(6, 27, true, 18, 26, &mecanumRearL_EncoderISR);
    void mecanumRearL_EncoderISR() { mecanumRearL.readEncoder(); };

    MOTOR mecanumRearR(7, 29, true, 19, 28, &mecanumRearR_EncoderISR);
    void mecanumRearR_EncoderISR() { mecanumRearR.readEncoder(); };

    void initMotors()
    {
        mecanumFrontL.begin();
        mecanumFrontR.begin();
        mecanumRearL.begin();
        mecanumRearR.begin();
    }

    /*****************************/
    /*   I N P U T   L O G I C   */
    /*****************************/

    // generic tri drive controls
    void procDrive(int joyLY, int joyLX, int joyRX)
    {
        // slater's mecanum magic
        // assumes x, y, r value input ranges are - 255 to + 255
        int m1Val = constrain(joyLY + joyLX + joyRX, -255, 255);
        int m2Val = constrain(joyLY - joyLX - joyRX, -255, 255);
        int m3Val = constrain(joyLY - joyLX + joyRX, -255, 255);
        int m4Val = constrain(joyLY + joyLX - joyRX, -255, 255);

        mecanumFrontL.setPower(m1Val);
        mecanumFrontR.setPower(m2Val);
        mecanumRearL.setPower(m3Val);
        mecanumRearR.setPower(m4Val);
    }
}
