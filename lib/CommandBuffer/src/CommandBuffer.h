#pragma once
#include <Arduino.h>
#include <Parser.h>

/// \class CommandBuffer <CommandBuffer.h>
/// \brief Circular buffer to hold move commands for the embroidery machine
///
/// Design decisions:
/// - Input is human readable
/// - As input only one character per call, to guaranee short loop times
///
/// Commands:
/// =========
///
/// Move:
/// m<float value in mm for x axis>;<float value in mm for y axis>;<float value in mm/s for speed>;
///
/// Disable Steppers:
/// d
///
/// Enable Steppers:
/// e
///
/// ... this is not the end of the list!
template<uint8_t COMMAND_COUNT>
class CommandBuffer
{
public:
    bool isFull() const
    {
        return _size == COMMAND_COUNT;
    }

    bool isEmpty() const
    {
        return _size == 0;
    }

    /// \brief Push the commands as string, but char by char ... keep everything reactive!
    /// \returns true if a command was completely received
    /// \remarks the caller has to take care about a full CommandBuffer ... see isFull(),
    ///          otherwise commands get overridden!
    bool push(char c)
    {
        auto* command = _parser.push(c);
        if(!command)
            return false;
        _buffer[_currentBufferIdx++] = *command;
        if(_currentBufferIdx == COMMAND_COUNT)
            _currentBufferIdx = 0;
        ++_size;
        return true;
    }

    const Command& pop()
    {
        if(isEmpty())
        {
            static const Command none{};
            return none;
        }
        int16_t idx = _currentBufferIdx - _size;
        if(idx < 0)
            idx = COMMAND_COUNT + idx;
        --_size;
        return _buffer[idx];
    }

    uint8_t size() const
    {
        return _size;
    }

private:
    Command _buffer[COMMAND_COUNT]{};
    uint8_t _currentBufferIdx{};
    uint8_t _size{};
    Parser _parser;
};