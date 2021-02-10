#include <Arduino.h>
#include <stdint.h>
#include <CommandBuffer.h>
#include "Mover.h"

Mover g_hoopMover;
CommandBuffer<20> g_commandBuffer;

void setup()
{
  Serial.begin(115200);
  Serial.write("# ");
}

void loop()
{
  if (!g_commandBuffer.isFull() && Serial.available())
    g_commandBuffer.push(Serial.read());

  if (!g_hoopMover.isRunning() && !g_commandBuffer.isEmpty())
  {
    auto &command = g_commandBuffer.pop();
    switch (command.command)
    {
    case Command::move: // move hoop
      g_hoopMover.moveTo(command.var0, command.var1, command.var3);
      break;
    case Command::disable_steppers: // disable steppers
      g_hoopMover.disable();
      break;
    case Command::enable_steppers: // enable steppers
      g_hoopMover.enable();
      break;
    default:
      break;
    }
  }
  g_hoopMover.run();
}