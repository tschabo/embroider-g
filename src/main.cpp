#include <Arduino.h>
#include <TimerOne.h>
#include "Mover.h"
#include <CircularBuffer.h>
#include "Parser.h"

CircularUnibuff g_buffer;

Mover hoopMover;

void move()
{
  hoopMover.run();
}

void setup()
{
  Timer1.initialize(100);
  Timer1.attachInterrupt(move);
}

void loop()
{
  
}