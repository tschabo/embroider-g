#include "Parser.h"
#include "stdlib.h"

Parser::Parser(ConfiguredRingbuffer &buff) : _buffer(buff)
{
}

void Parser::parseCommand(char command)
{
    switch (command)
    {
    case 'm': //move
        _state = findX;
        _currentCommandBuffer.command = command;
        break;
    case 'd': // disable steppers
    case 'e': // enable steppers
        _buffer.pushFront().command = command;
        _state = findStart;
        break;
    }
}

bool Parser::parseFloat(char floatPart, float &thePlaceToPut)
{
    if (floatPart == ';')
    {
        thePlaceToPut = atof(_scratch_buffer.data());
        _scratch_buffer.clear();
        return true; // the caller has to set the new State
    }
    if (_scratch_buffer.full() || ((floatPart < '0' || floatPart > '9') && floatPart != '.'))
    {
        _state = findStart;
        _scratch_buffer.clear();
        return false;
    }
    _scratch_buffer.push_back(floatPart);
    return false;
}

bool Parser::push(char c)
{
    if (_buffer.isFull())
        return false;

    switch (_state)
    {
    case findStart:
        if (c != '>')
            break;
        _state = evalCommand;
        break;
    case evalCommand:
        parseCommand(c);
        break;
    case findX:
        if (parseFloat(c, _currentCommandBuffer.var0))
            _state = findY;
        break;
    case findY:
        if (parseFloat(c, _currentCommandBuffer.var1))
            _state = findSpeed;
        break;
    case findSpeed:
        if (parseFloat(c, _currentCommandBuffer.var3))
        {
            _buffer.pushFront() = _currentCommandBuffer;
            _state = findStart;
        }
        break;
    }
    return true;
}