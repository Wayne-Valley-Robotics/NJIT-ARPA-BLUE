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
    void m1EncoderISR();
    void m2EncoderISR();
    void m3EncoderISR();
    void m4EncoderISR();

    // pin, invert, encoderPinA, encoderPinB
    // only the first two are required
    MOTOR m1(9, 8, 0, 2, 3, &m1EncoderISR);
    void m1EncoderISR() { m1.readEncoder(); };

    MOTOR m2(10, 9, 0, 18, 19, &m2EncoderISR);
    void m2EncoderISR() { m2.readEncoder(); };

    MOTOR m3(11, 10, 0, 20, 21, &m3EncoderISR);
    void m3EncoderISR() { m3.readEncoder(); };

    MOTOR m4(12, 11, 0, 20, 21, &m4EncoderISR);
    void m4EncoderISR() { m4.readEncoder(); };

    void initMotors() // abstracted to drive_interface to avoid depending on this header in main.cpp
    {
        m1.begin();
        m2.begin();
        m3.begin();
        m4.begin();
    }

    /*      Directions:     */

    // I'm not sure how the different algorithms would interact with this. it feels too dumb almost...
    // idk it's up to you lol
    // feel free to scrap the functions below
    void forward(int power)
    {
        m1.setPower(power);
        m2.setPower(power);
        m3.setPower(power);
        m4.setPower(power);
    }
    void backward(int power)
    {
        m1.setPower(-power);
        m2.setPower(-power);
        m3.setPower(-power);
        m4.setPower(-power);
    }

    // TODO: add other directions, like strafing and rotating

}
