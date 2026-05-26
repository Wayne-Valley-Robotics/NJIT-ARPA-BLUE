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

void MOTOR::setPower(int _targetPower)
{
    // targetSpeed = 0; // we want to set the power, ensure target speed is nothing.
    // invert direction if set
    _targetPower *= invertMultiplier;
    bool direction = _targetPower > 0;
    // waste of cpu cycles, but I'd rather it predictably rollover
    uint8_t pwmPower = map(abs(_targetPower), 0, MAX_SPEED, 0, 255);
    if (abs(_targetPower) > 255)
    {
        Serial.println("Motor power rollover. Did you mean to do that?");
    }

    analogWrite(pwm, pwmPower);
    digitalWrite(dir, direction);
}

void MOTOR::setSpeed(int _targetSpeed)
{
    // say speed, motor go it!
    // repeatedly call this

    // coast if 0
    // if (_targetSpeed == 0)
    // {
    //     setPower(0);
    //     return;
    // }

    // apply inversion
    _targetSpeed *= invertMultiplier;

    // estimate initial speed
    // if (_targetSpeed != targetSpeed)
    //     currentPower = MAX_ENC_SPEED / MAX_SPEED * targetSpeed;

    // set target speed
    targetSpeed = _targetSpeed;
}

// called from calculateEncoderSpeed
void MOTOR::adjustSpeed()
{

    // Serial.print("Encoder Speed: ");
    // Serial.print(getEncoderSpeed());
    // Serial.print("\tEncoder Count: ");
    // Serial.print(getEncoderCount());
    // Serial.print("\tCurrent Power: ");
    // Serial.print(currentPower);
    // Serial.print("\tTarget Speed: ");
    // Serial.print(targetSpeed);
    // Serial.println();

    int regMultiplier = targetSpeed > 0 ? 1 : -1;

    if (getEncoderSpeed() < targetSpeed)
    { // if motor speed not enough, increase power to it by a little bit
        currentPower = min(currentPower + SPEED_ADJUSTMENT_INCREMENT, MAX_SPEED);
        setPower(currentPower * regMultiplier);
        return;
    }

    if (getEncoderSpeed() > targetSpeed)
    {
        currentPower = max(currentPower - SPEED_ADJUSTMENT_INCREMENT, -MAX_SPEED);
        setPower(currentPower * regMultiplier);
        return;
    }
}

long MOTOR::getEncoderCount()
{
    noInterrupts(); // atomic guards
    long count = encoderCount * invertMultiplier;
    interrupts();

    return count;
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
    long encoderDisplacement = abs(currentEncoderCount - lastEncoderCount); // change in encoder count since last function call

    // Calculate the speed of the motor (S = d/t) in pulses per second
    encoderSpeed = (encoderDisplacement * 1000L) / delta;

    if (targetSpeed != 0)
        adjustSpeed();

    // Cache values for next time
    lastTime = currentTime;
    lastEncoderCount = currentEncoderCount;
}

long MOTOR::getEncoderSpeed()
{
    return encoderSpeed;
}
