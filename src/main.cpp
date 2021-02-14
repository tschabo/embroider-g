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

bool isCommandComplete{};

void loop()
{
  isCommandComplete = false;
  if (Serial.available() && !g_commandBuffer.isFull())
    isCommandComplete = g_commandBuffer.push(Serial.read());

  if (g_commandBuffer.isFull() && !wait)
  {
    // this is the case, when a command has completed and the buffer got full
    Serial.print("!"); // signal buffer full
    wait = true;
  }
  else if (!g_commandBuffer.isFull() && wait && !Serial.available())
  {
    // this is the case when the buffer was previosly full and the caller is waiting for sending more data.
    // but before we let the caller send more data ... read the pending data (if any) from the serial buffer.
    Serial.print("+"); // signal there is space left
    wait = false;
  }
  else if (isCommandComplete)
  {
    Serial.print(">"); // normal ACK
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