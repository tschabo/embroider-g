#include "SimpleStepper.h"

SimpleStepper::SimpleStepper(int enablePin,
                             int dirPin,
                             int stepPin,
                             uint32_t stepsPerM) : _enablePin(enablePin),
                                                    _dirPin(dirPin),
                                                    _stepPin(stepPin),
                                                    _stepsPerM(stepsPerM)
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

uint32_t SimpleStepper::stepsToGo() const
{
    return _stepsToGo;
}

void SimpleStepper::moveAbs(float pos)
{
    int32_t posInStepsFromZero{(pos * (float)_stepsPerM) / 1000.0};
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
    digitalWrite(_dirPin, (_direction == direction::pos) ? HIGH : LOW);
}

void SimpleStepper::moveSteps(uint32_t steps, direction dir){
    _position=0;
    _direction = dir;
    _stepsToGo = steps;
}

void SimpleStepper::setSpeed(float speed)
{
    _speed = (speed * (float)_stepsPerM) / 1000; // ticks/sec
    _stepInterval = 1000000 / _speed;
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