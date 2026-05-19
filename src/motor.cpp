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

void MOTOR::setPower(int targetPower)
{
    // invert direction if set
    targetPower *= invertMultiplier;
    bool direction = targetPower > 0;
    // waste of cpu cycles, but I'd rather it predictably rollover
    uint8_t pwmPower = abs(targetPower);
    if (pwmPower > 255)
    {
        Serial.println("Motor power rollover. Did you mean to do that?");
    }

    analogWrite(pwm, pwmPower);
    digitalWrite(dir, direction);
}

void MOTOR::setSpeed(int targetSpeed) {
    // say speed, motor go it!
    // repeatedly call this 
    static int lastPower;
    int currentPower = lastPower;
    if (getEncoderCount() < targetSpeed) { // if motor speed not enough, increase power to it by a little bit
        currentPower++; // or however much you want to increase it by
    } else if (getEncoderCount() > targetSpeed) {
        currentPower--; // or however much you want to decrease it by
    }
    lastPower = currentPower;
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

void MOTOR::calculateEncoderSpeed()
{
    // Calculate Delta (change in time) since last function call.
    static unsigned long timeStamp;
    unsigned long currentTime = millis(); // cache millis() function call to save cpu cycles
    unsigned long delta = currentTime - timeStamp; // change in time since last function call

    // Calculate encoder displacement since last function call.
    static unsigned long lastEncoderCount;
    unsigned long currentEncoderCount = getEncoderCount();
    unsigned long encoderDisplacement = currentEncoderCount - lastEncoderCount; // change in encoder count since last function call

    // Calculate the speed of the motor
    encoderSpeed = (encoderDisplacement * 1000L) / delta; // ticks per second
    
    // reset for next caching
    timeStamp = currentTime;
    lastEncoderCount = currentEncoderCount;
}

long MOTOR::getEncoderSpeed()
{
    return encoderSpeed;
}
