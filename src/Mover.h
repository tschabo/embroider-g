#pragma once
#include <stdint.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

// convinience class for hoop positioning
// ... no acceleration needed? just move as fast as possible to toe next position
// 
class Mover
{
public:
    Mover();

    // mm and mm/s
    void moveTo(float x, float y, float speed);

    void enable();
    void disable();

    // has to be called frequently ... fast !!!
    void run();

private:
    AccelStepper _stepperX;
    AccelStepper _stepperY;
    MultiStepper _stepperGroup;
    uint16_t _ticks_per_mm = 100; // GT2 2mm + 16 teeth pulley + 1/16 microstepping
};
