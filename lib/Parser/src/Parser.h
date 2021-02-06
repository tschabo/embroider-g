#pragma once

#include "RingBuffer.h"

template <uint8_t SIZE>
class MyBuffer
{
public:

    bool push_back(char c)
    {
        if (full())
            return false;
        _buffer[_size] = c;
        ++_size;
        return true;
    }

    void clear()
    {
        _size = 0;
    }

    uint8_t size() const
    {
        return _size;
    }

    bool full() const
    {
        return _size == SIZE;
    }

    char *data()
    {
        return &_buffer[0];
    }

private:
    char _buffer[SIZE];
    uint8_t _size{};
};

#define SINGLE_BUFFER_SIZE (1 + sizeof(float) * 3)

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
    MyBuffer<50> _scratch_buffer{};
    Command _currentCommandBuffer{};
    void parseCommand(char command);
    bool parseFloat(char floatPart, float &thePlaceToPut); // returns true if finished parsing
};
