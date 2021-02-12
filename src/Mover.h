#pragma once
#include <stdint.h>
#include <SimpleStepper.h>

// convinience class for hoop positioning
// ... no acceleration needed?! just move as fast as possible to the next position
// 
class Mover
{
public:
    Mover();

    // mm and mm/s
    void moveTo(float x, float y, float speed);

    // setting the current position ... wherever the hoop currently is
    void setPosition(float x, float y);

    // enable/disable Stepperdriver
    void enable();
    void disable();

    bool isRunning() const;

    // has to be called frequently ... fast !!!
    void run();

private:
    SimpleStepper _stepperX;
    SimpleStepper _stepperY;
    const uint16_t _ticks_per_mm = 100; // GT2 2mm + 16 teeth pulley + 1/16 microstepping
};
