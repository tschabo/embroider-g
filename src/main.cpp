#include <Arduino.h>
#include <stdint.h>
#include <CommandBuffer.h>
#include "Mover.h"

Mover g_hoopMover;
CommandBuffer<20> g_commandBuffer;

void setup()
{
  Serial.begin(115200);
}

bool wait = false;

void loop()
{
  if (!g_commandBuffer.isFull() && Serial.available())
    g_commandBuffer.push(Serial.read());

  if (g_commandBuffer.isFull() && !wait)
  {
    Serial.print("!"); // signal buffer full
    wait = true;
  }
  else if(!g_commandBuffer.isFull() && wait && !Serial.available())
  {
    Serial.print("+"); // signal there is space left
    wait = false;
  }

  if (!g_hoopMover.isRunning() && !g_commandBuffer.isEmpty())
  {
    auto &command = g_commandBuffer.pop();
    switch (command.command)
    {
    case Command::move:
      g_hoopMover.moveTo(command.var0, command.var1, command.var3);
      break;
    case Command::disable_steppers:
      g_hoopMover.disable();
      break;
    case Command::enable_steppers:
      g_hoopMover.enable();
      break;
    default:
      break;
    }
  }
  g_hoopMover.run();
}