#pragma once

#include <Array.h>
#include <CircularBuffer.h>

struct unibuff
{
    char _[sizeof(float) * 3 + 1]{};
};
using CircularUnibuff = CircularBuffer<unibuff, 10>;

class Parser
{
public:
    Parser(CircularUnibuff &buff);

    // feeding bytewise ... returns false if Buffer is full
    bool push(char c);

private:
    CircularUnibuff &_buffer;
    enum
    {
        findStart,
        evalCommand,
        findX,
        findY,
        findSpeed,
    } _state{findStart};
    Array<char, 50> _scratch_buffer{};
    unibuff _currentCommandBuffer{};
    void parseCommand(char command);
    bool parseFloat(char floatPart, float &thePlaceToPut); // returns true if finished parsing
};
