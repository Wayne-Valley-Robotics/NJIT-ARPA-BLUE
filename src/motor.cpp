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

    // Do not change this without understanding the readEncoder implementation.
    attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, CHANGE);
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
    return encoderCount * invertMultiplier; // invert encoder reading!!
}

void MOTOR::readEncoder() // called every time pin A changes
{
    // cache encoder pin values
    bool a = digitalRead(encoderPinA);
    bool b = digitalRead(encoderPinB);
    // a==b == forward
    // a!=b == backward
    if (a == b) // forward
        encoderCount += 1;
    else s// backward
        encoderCount -= 1;
}

void MOTOR::resetEncoder()
{
    encoderCount = 0;
}

void MOTOR::calculateEncoderSpeed()
{
    // Calculate delta (change in time) since last function call.
    static unsigned long lastTime;
    unsigned long currentTime = millis(); // cache millis() function call to save cpu cycles
    unsigned long delta = currentTime - lastTime; // change in time since last function call

    // Calculate encoder displacement since last function call.
    static unsigned long lastEncoderCount;
    unsigned long currentEncoderCount = getEncoderCount();
    unsigned long encoderDisplacement = currentEncoderCount - lastEncoderCount; // change in encoder count since last function call

    // Calculate the speed of the motor (S = d/t)
    encoderSpeed = (encoderDisplacement * 1000L) / delta; // ticks per second

    // Cache values for next time
    lastTime = currentTime;
    lastEncoderCount = currentEncoderCount;
}

long MOTOR::getEncoderSpeed()
{
    return encoderSpeed;
}
