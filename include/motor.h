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

public:
    MOTOR(int pwm, int dir, bool invert, int encoderPinA, int encoderPinB, void (*encoderISR)());
    MOTOR(int pwm, int dir, bool invert);
    MOTOR(int pwm, int dir);
    void begin();
    // set motor power from -255 to 255
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
    void resetEncoder();
    // return encoder ticks per second
    long getEncoderSpeed();
    // DO NOT CALL under normal use. TODO: find a way to call this regularly
    void calculateEncoderSpeed();
};
