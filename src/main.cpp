#include <Arduino.h>
#include "Parser.h"
#include <stdint.h>
#include "Mover.h"

Mover g_hoopMover;
ConfiguredRingbuffer g_buffer{};
Parser g_parser(g_buffer);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  if (!g_buffer.isFull() && Serial.available())
    g_parser.push(Serial.read());

  if (!g_hoopMover.isRunning() && !g_buffer.isEmpty())
  {
    auto &command = g_buffer.popBack();
    switch (command.command)
    {
    case 'm': // move hoop
      g_hoopMover.moveTo(command.var0, command.var1, command.var3);
      break;
    case 'd': // disable steppers
      g_hoopMover.disable();
      break;
    case 'e': // enable steppers
      g_hoopMover.enable();
      break;
    }
  }
  g_hoopMover.run();
}