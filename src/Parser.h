#pragma once

#include <Array.h>
#include "RingBuffer.h"

#define SINGLE_BUFFER_SIZE (1+sizeof(float)*3)

using ConfiguredRingbuffer = RingBuffer<20>;

class Parser
{
public:
    Parser(ConfiguredRingbuffer &buff);

    // feeding bytewise ... returns false if Buffer is full
    bool push(char c);

private:
    ConfiguredRingbuffer &_buffer;
    enum
    {
        findStart,
        evalCommand,
        findX,
        findY,
        findSpeed,
    } _state{findStart};
    Array<char, 50> _scratch_buffer{};
    Command _currentCommandBuffer{};
    void parseCommand(char command);
    bool parseFloat(char floatPart, float &thePlaceToPut); // returns true if finished parsing
};
