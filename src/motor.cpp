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
    this->encoderSpeed = 0;
    this->lastEncoderCount = 0;
    this->lastTime = 0;
}

MOTOR::MOTOR(int pwm, int dir, bool invert)
{
    this->pwm = pwm;
    this->dir = dir;
    this->invertMultiplier = invert ? -1 : 1;
    this->encoderISR = nullptr;
    this->encoderCount = 0;
    this->encoderSpeed = 0;
    this->lastEncoderCount = 0;
    this->lastTime = 0;
}

MOTOR::MOTOR(int pwm, int dir)
{
    this->pwm = pwm;
    this->dir = dir;
    invertMultiplier = 1;
    this->encoderISR = nullptr;
    this->encoderCount = 0;
    this->encoderSpeed = 0;
    this->lastEncoderCount = 0;
    this->lastTime = 0;
}

void MOTOR::begin()
{
    pinMode(pwm, OUTPUT);
    pinMode(dir, OUTPUT);
    if (encoderISR != nullptr)
    {
        pinMode(encoderPinA, INPUT_PULLUP);
        pinMode(encoderPinB, INPUT_PULLUP);
        // Do not change this without understanding the readEncoder implementation.
        attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, CHANGE);
    }
}

// takes 0-255
void MOTOR::setPower(int power)
{
    // invert direction if set
    power *= invertMultiplier;
    bool direction = power > 0;
    // waste of cpu cycles, but I'd rather it predictably rollover
    uint8_t pwmPower = abs(power);
    if (abs(power) > 255)
    {
        Serial.println("Motor power rollover. Did you mean to do that?");
    }

    analogWrite(pwm, pwmPower);
    digitalWrite(dir, direction);
}

long MOTOR::getEncoderCount()
{
    noInterrupts(); // atomic guards
    long count = encoderCount;
    interrupts();

    return count * invertMultiplier;
}

void MOTOR::readEncoder() // called every time pin A changes
{
    // cache encoder pin values
    bool a = digitalRead(encoderPinA);
    bool b = digitalRead(encoderPinB);
    // a==b == forward
    // a!=b == backward
    if (a == b) // forward
    {
        encoderCount += 1;
    }
    else // backward
    {
        encoderCount -= 1;
    }
}

void MOTOR::resetEncoder()
{
    encoderCount = 0;
}

void MOTOR::calculateEncoderSpeed()
{
    // Calculate delta (change in time) since last function call.
    unsigned long currentTime = millis();         // cache millis() function call to save cpu cycles
    unsigned long delta = currentTime - lastTime; // change in time since last function call
    if (delta < minEncCalcDelta)                  // Only calculate if at least MIN_ENC_CALC_DELTA ms have passed for better resolution
        return;

    // Calculate encoder displacement since last function call.
    long currentEncoderCount = getEncoderCount();
    long encoderDisplacement = currentEncoderCount - lastEncoderCount; // change in encoder count since last function call
    int reg = encoderDisplacement > 0 ? 1 : -1;

    // Calculate the speed of the motor (S = d/t) in pulses per second
    encoderSpeed = (abs(encoderDisplacement) * 1000L) / delta * reg;

    // Cache values for next time
    lastTime = currentTime;
    lastEncoderCount = currentEncoderCount;
}

long MOTOR::getEncoderSpeed()
{
    return encoderSpeed;
}
