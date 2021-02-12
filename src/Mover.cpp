#include "Mover.h"

#ifdef __AVR_ATmega1284P__
// The X Stepper pins
#define STEPPER1_DIR_PIN 21
#define STEPPER1_STEP_PIN 15
#define STEPPER1_ENABLE_PIN 14
// The Y stepper pins
#define STEPPER2_DIR_PIN 23
#define STEPPER2_STEP_PIN 22
#define STEPPER2_ENABLE_PIN 14
#else 
// nano cnc shield
// The X Stepper pins
#define STEPPER1_DIR_PIN 4
#define STEPPER1_STEP_PIN 7
#define STEPPER1_ENABLE_PIN 8
// The Y stepper pins
#define STEPPER2_DIR_PIN 3
#define STEPPER2_STEP_PIN 6
#define STEPPER2_ENABLE_PIN 8
#endif

Mover::Mover() : _stepperX(SimpleStepper(STEPPER1_ENABLE_PIN, STEPPER1_DIR_PIN, STEPPER1_STEP_PIN, 3125*16)),
                 _stepperY(SimpleStepper(STEPPER2_ENABLE_PIN, STEPPER2_DIR_PIN, STEPPER2_STEP_PIN, 3125*16))
{
    _stepperX.disable();
    _stepperY.disable();
}

void Mover::moveTo(float x, float y, float speed)
{
    // coreXY
    auto x_ = (x-y)*2;
    auto y_ = (x+y)*2;
    _stepperX.moveAbs(x_);
    _stepperY.moveAbs(y_);
    const float stepsX = _stepperX.stepsToGo();
    const float stepsY = _stepperY.stepsToGo();
    float speedX{};
    float speedY{};
    if(stepsX > stepsY)
    {
        speedX = speed;
        speedY = stepsY / stepsX * speed;
    }
    else if(stepsY > stepsX)
    {
        speedY = speed;
        speedX = stepsX / stepsY * speed;
    }
    else
    {
        speedY = speedX = speed;
    }
    _stepperX.setSpeed(speedX);
    _stepperY.setSpeed(speedY);
}

void Mover::setPosition(float x, float y)
{
    _stepperX.setPosition(static_cast<long>(x));
    _stepperY.setPosition(static_cast<long>(y));
}

void Mover::enable()
{
    _stepperX.enable();
    _stepperY.enable();
}

void Mover::disable()
{
    _stepperX.disable();
    _stepperY.disable();
}

bool Mover::isRunning() const
{
    return _stepperX.stepsToGo() != 0 || _stepperY.stepsToGo() != 0;
}

void Mover::run()
{
    if(!isRunning())
        return;
        
    _stepperX.run();
    _stepperY.run();
}