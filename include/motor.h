#pragma once
#include <Arduino.h>

class MOTOR
{
private:
    uint8_t pwm;
    uint8_t dir;
    int invertMultiplier;
    uint8_t encoderPinA; // this must be connected to an interrupt pin
    uint8_t encoderPinB; // connect to any digital pin
    void (*encoderISR)();
    volatile long encoderCount;
    long encoderSpeed;
    long lastEncoderCount;
    unsigned long lastTime;

public:
    // minimum delay in between processing of calculateEncoderSpeed()
    constexpr static long minEncCalcDelta = 200; // this is a safe number for testing, feel free to adjust but if you do please test it thoroughly (eyeball it ig idk lol print it to serial)

    MOTOR(int pwm, int dir, bool invert, int encoderPinA, int encoderPinB, void (*encoderISR)());
    MOTOR(int pwm, int dir, bool invert);
    MOTOR(int pwm, int dir);
    // This function is separate so that you can create MOTOR objects before setup() and set pinModes later so as to not interfere with boot processes.
    void begin();
    // set motor power from -255 to 255. Make sure you map your inputs!
    void setPower(int power);
    // manually set pwm pin from 0 to 255 and dir pin as a boolean
    void setPins(uint8_t pwm, bool dir);
    // set-and-forget motor inversion, will apply to every function EXCEPT setPins
    void setMotorInvert(bool invert);

    // encoders

    // return signed encoder count
    long getEncoderCount();

    // DO NOT CALL under normal use. Internal ISR function to be attached to interrupt service routine if you wish to use encoders.
    void readEncoder();
    // set encoderCount to 0. Use this to "center" a motor in a trusted position, like after hitting a limit switch.
    void resetEncoder();
    // return encoder ticks per second
    long getEncoderSpeed();
    // Call this at the very least minCalcDelta ms apart (as often as possible)
    void calculateEncoderSpeed();
};
