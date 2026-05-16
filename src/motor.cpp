#include "motor.h"

MOTOR::MOTOR(int pwm, int dir, bool invert, int encoderPinA, int encoderPinB, void (*encoderISR)())
{
    this->pwm = pwm;
    this->dir = dir;
    this->encoderPinA = encoderPinA;
    this->encoderPinB = encoderPinB;
    this->encoderCount = 0;
    this->invertMultiplier = invert ? -1 : 1;
    this->encoderISR = encoderISR;
}

MOTOR::MOTOR(int pwm, int dir, bool invert)
{
    this->pwm = pwm;
    this->dir = dir;
    this->invertMultiplier = invert ? -1 : 1;
    pinMode(pwm, OUTPUT);
    pinMode(dir, OUTPUT);
}

MOTOR::MOTOR(int pwm, int dir)
{
    this->pwm = pwm;
    this->dir = dir;
    pinMode(pwm, OUTPUT);
    pinMode(dir, OUTPUT);
    invertMultiplier = 1;
}

void MOTOR::begin()
{
    pinMode(pwm, OUTPUT);
    pinMode(dir, OUTPUT);
    pinMode(encoderPinA, INPUT);
    pinMode(encoderPinB, INPUT);

    // Do not change this without modifying the readEncoder implementation.
    attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, CHANGE);
}

void MOTOR::setPower(int16_t power)
{

    // invert direction if set
    power *= invertMultiplier;
    bool direction = power > 0;
    // waste of cpu cycles, but I'd rather it predictably rollover
    uint8_t pwmPower = abs(power);
    if (pwmPower > 255)
    {
        Serial.println("Motor power rollover. Did you mean to do that?");
    }

    analogWrite(pwm, pwmPower);
    digitalWrite(dir, direction);
}

long MOTOR::getEncoderCount()
{
    return encoderCount;
}
void MOTOR::readEncoder() // called every time pin A changes
{
    // cache encoder pin values
    // bool a = digitalRead(encoderPinA);
    bool b = digitalRead(encoderPinB);
    // a1 b0 == forward
    // a0 b1 == backward
    if (b)
    {
        encoderCount++;
        // encoderDirection = true; // waste of cpu cycles
    }
    else
    {
        encoderCount--;
        // encoderDirection = false;
    }
}

void MOTOR::resetEncoder()
{
    encoderCount = 0;
}
