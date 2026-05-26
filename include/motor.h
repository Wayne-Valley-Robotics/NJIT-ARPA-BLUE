#pragma once
#include <Arduino.h>

class MOTOR
{
public: // settings

    const static int MAX_SPEED = 255;
    // 60:1 - 1,140cpr@100rpm | 40:1 - 960cpr@150rpm | 20:1 - 480cpr@300rpm
    const static int MAX_ENC_SPEED = 1440;
    const static  int SPEED_ADJUSTMENT_INCREMENT = 2;
    // minimum delay in between processing of encoder speed calculation and adjustment
    const static long minEncCalcDelta = 20;

private:
    uint8_t pwm;
    uint8_t dir;
    int8_t invertMultiplier;
    uint8_t encoderPinA; // this must be connected to an interrupt pin
    uint8_t encoderPinB; // connect to any digital pin
    void (*encoderISR)();
    volatile long encoderCount;
    long encoderSpeed;
    long lastEncoderCount;
    unsigned long lastTime;
    int targetSpeed;
    int currentPower;
    void adjustSpeed();

public:
    MOTOR(int pwm, int dir, bool invert, int encoderPinA, int encoderPinB, void (*encoderISR)());
    MOTOR(int pwm, int dir, bool invert);
    MOTOR(int pwm, int dir);
    void begin();
    // set motor power from -255 to 255. Make sure you map your inputs!
    void setPower(int targetPower);
    // set motor speed in encoder pulses per second. Make sure you map your inputs!
    void setSpeed(int targetSpeed);
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
