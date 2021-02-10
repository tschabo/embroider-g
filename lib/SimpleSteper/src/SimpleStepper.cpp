#include "SimpleStepper.h"

SimpleStepper::SimpleStepper(int enablePin,
                             int dirPin,
                             int stepPin,
                             uint16_t stepsPerMm) : _enablePin(enablePin),
                                                    _dirPin(dirPin),
                                                    _stepPin(stepPin),
                                                    _stepsPerM(stepsPerMm)
{
    pinMode(_enablePin, OUTPUT);
    digitalWrite(_enablePin, HIGH); // disable is default
    pinMode(_dirPin, OUTPUT);
    pinMode(_stepPin, OUTPUT);
    digitalWrite(_stepPin, LOW);
}

void SimpleStepper::enable() const
{
    digitalWrite(_enablePin, LOW);
}

void SimpleStepper::disable() const
{
    digitalWrite(_enablePin, HIGH);
}

uint16_t SimpleStepper::stepsToGo() const
{
    return _stepsToGo;
}

void SimpleStepper::moveAbs(float pos)
{
    uint16_t posInStepsFromZero{(pos * (float)_stepsPerM) / 1000.0};
    if (posInStepsFromZero > _position)
    {
        _stepsToGo = posInStepsFromZero - _position;
        _direction = direction::pos;
    }
    else if (posInStepsFromZero < _position)
    {
        _stepsToGo = _position - posInStepsFromZero;
        _direction = direction::neg;
    }
    else
    {
        _stepsToGo = 0;
    }
}

void SimpleStepper::setSpeed(float speed)
{
    _speed = (speed * (float)_stepsPerM) / 1000; // ticks/sec
    _stepInterval = 1000000 / _speed;
    Serial.print("_stepInterval: ");
    Serial.println(_stepInterval);
}

void SimpleStepper::setPosition(float pos)
{
    _position = (pos * (float)_stepsPerM)/1000.0;
}

void SimpleStepper::run()
{
    if (_stepsToGo == 0)
        return;

    auto current = micros();

    if ((current - _lastStepTime) >= _stepInterval)
    {
        // step !!!
        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(2);
        digitalWrite(_stepPin, LOW);
        switch (_direction)
        {
        case direction::neg:
            --_position;
            break;
        default:
            ++_position;
            break;
        }
        --_stepsToGo;
        _lastStepTime = current;
    }
}