#pragma once
#include <Arduino.h>

/// \brief SimpleStepper controls a stepper without acceleration
/// We try to keep it simple, to drive the stepper as fast as possible.
/// Only implement acceleration if we need it to reach higher speeds!
class SimpleStepper
{
public:
    SimpleStepper(int enablePin,
                  int dirPin,
                  int stepPin,
                  uint16_t stepsPerMm);

    void enable() const;
    void disable() const;

    void moveAbs(float pos);
    void setSpeed(float speed);
    void setPosition(float pos);

    uint16_t stepsToGo() const;

    void run();

private:
    const int _enablePin;
    const int _dirPin;
    const int _stepPin;
    const uint16_t _stepsPerM;

    // internal we calculate everything in steps
    uint16_t _position{};
    uint16_t _stepsToGo{};
    uint16_t _speed{};
    enum class direction{
        pos,
        neg,
    } _direction{direction::pos};

    // time is in micro seconds
    unsigned long _stepInterval{};
    unsigned long _lastStepTime{};
};