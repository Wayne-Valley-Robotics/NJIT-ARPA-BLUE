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
    this->targetSpeed = 0;
    this->currentPower = 0;
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
    this->targetSpeed = 0;
    this->currentPower = 0;
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
    this->targetSpeed = 0;
    this->currentPower = 0;
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
    int power = constrain(_targetPower, -MAX_SPEED, MAX_SPEED);
    power *= invertMultiplier;
    bool direction = power > 0;
    uint8_t pwmPower = map(abs(power), 0, MAX_SPEED, 0, 255);
    if (abs(_targetPower) > MAX_SPEED)
    {
        Serial.println("Motor power rollover. Did you mean to do that?");
    }

    analogWrite(pwm, pwmPower);
    digitalWrite(dir, direction);
}

void MOTOR::setSpeed(int _targetSpeed)
{
    // Set the logical target speed.
    // Feedback (encoderSpeed) is already inversion-corrected.

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

    if (getEncoderSpeed() < targetSpeed)
    { // if motor speed not enough, increase power to it by a little bit
        currentPower = min(currentPower + SPEED_ADJUSTMENT_INCREMENT, MAX_SPEED);
        setPower(currentPower);
        return;
    }

    if (getEncoderSpeed() > targetSpeed)
    {
        currentPower = max(currentPower - SPEED_ADJUSTMENT_INCREMENT, -MAX_SPEED);
        setPower(currentPower);
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

    // Calculate logical encoder displacement and speed
    long currentEncoderCount = getEncoderCount();
    // speed = (change in counts * 1000ms) / change in time
    encoderSpeed = ((currentEncoderCount - lastEncoderCount) * 1000L) / (long)delta;

    // If target is 0, stop immediately and reset the adjustment power
    if (targetSpeed != 0)
        adjustSpeed();
    else
    {
        setPower(0);
        currentPower = 0;
    }
    // Cache values for next time
    lastTime = currentTime;
    lastEncoderCount = currentEncoderCount;
}

long MOTOR::getEncoderSpeed()
{
    noInterrupts(); // atomic guards
    long speed = encoderSpeed;
    interrupts();

    return speed;
}
