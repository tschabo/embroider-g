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
#else // ATMEGA32U4 dont know what the define is
// The X Stepper pins
#define STEPPER1_DIR_PIN 7
#define STEPPER1_STEP_PIN 6
#define STEPPER1_ENABLE_PIN 8
// The Y stepper pins
#define STEPPER2_DIR_PIN 14
#define STEPPER2_STEP_PIN 15
#define STEPPER2_ENABLE_PIN 16
#endif

namespace
{
    void setSpeed(AccelStepper &stepper, float speed)
    {
        stepper.setMaxSpeed(speed);
        stepper.setSpeed(stepper.distanceToGo() < 0 ? -speed : speed);
    }
} // namespace

Mover::Mover() : _stepperX(AccelStepper(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN)),
                 _stepperY(AccelStepper(AccelStepper::DRIVER, STEPPER1_STEP_PIN, STEPPER1_DIR_PIN))
{
    _stepperX.setEnablePin(STEPPER1_ENABLE_PIN);
    _stepperX.disableOutputs();
    _stepperY.setEnablePin(STEPPER2_ENABLE_PIN);
    _stepperY.disableOutputs();
    _stepperGroup.addStepper(_stepperX);
    _stepperGroup.addStepper(_stepperY);
}

void Mover::moveTo(float x, float y, float speed)
{
    long position[]{static_cast<long>(x * _ticks_per_mm), static_cast<long>(y * _ticks_per_mm)};
    _stepperGroup.moveTo(position);
    setSpeed(_stepperX, speed);
    setSpeed(_stepperY, speed);
}

void Mover::setPosition(float x, float y)
{
    _stepperX.setCurrentPosition(static_cast<long>(x * _ticks_per_mm));
    _stepperY.setCurrentPosition(static_cast<long>(y * _ticks_per_mm));
}

void Mover::enable()
{
    _stepperX.enableOutputs();
    _stepperY.enableOutputs();
}

void Mover::disable()
{
    _stepperX.disableOutputs();
    _stepperY.disableOutputs();
}

bool Mover::isRunning()
{
    return _stepperX.isRunning() || _stepperY.isRunning();
}

void Mover::run()
{
    if (isRunning())
        _stepperGroup.run();
}