#pragma once
#include <stdint.h>
#include <Array.h>

struct Command{
    char command{};
    float var0{};
    float var1{};
    float var3{};
};

template <uint8_t ELEMENT_COUNT>
class RingBuffer
{
public:
    Command& pushFront()
    {
        auto &out{_commandBuffer[_head]};
        out = Command{};
        if (_head == ELEMENT_COUNT)
            _head = 0;
        else
            ++_head;
        ++_size;
        return out;
    }
    
    Command& popBack()
    {
        int16_t pos = _head - _size;
        if (pos < 0)
            pos += ELEMENT_COUNT;
        --_size;
        return _commandBuffer[pos];
    }

    bool isEmpty() const
    {
        return _size == 0;
    }

    bool isFull() const
    {
        return _size == ELEMENT_COUNT;
    }

private:
    Command _commandBuffer[ELEMENT_COUNT]{};
    uint8_t _head{0};
    uint8_t _size{0};
};
