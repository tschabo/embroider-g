#include "Parser.h"

Parser::Parser(CircularUnibuff &buff) : _buffer(buff)
{
}

void Parser::parseCommand(char command)
{
    switch (command)
    {
    case 'm': //move
        _state = findX;
        _currentCommandBuffer._[0] = command;
        break;
    }
}

bool Parser::parseFloat(char floatPart, float &thePlaceToPut)
{
    if (floatPart == ';')
    {
        float &tmp = *reinterpret_cast<float *>(&_currentCommandBuffer._[1]);
        tmp = atof(_scratch_buffer.data());
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
    if(_buffer.isFull())
        return false;
        
    switch (_state)
    {
    case findStart:
        if(c != '>')
            break;
        _state = evalCommand;
        break;
    case evalCommand:
        parseCommand(c);
        break;
    case findX:
        if(parseFloat(c, *reinterpret_cast<float*>(&_currentCommandBuffer._[1])))
            _state = findY;
        break;
    case findY:
        if(parseFloat(c, *reinterpret_cast<float*>(&_currentCommandBuffer._[1+sizeof(float)])))
            _state = findSpeed;
        break;
    case findSpeed:
        if(parseFloat(c, *reinterpret_cast<float*>(&_currentCommandBuffer._[11+sizeof(float)+sizeof(float)])))
            _state = findStart;
        break;
    }
    return true;
}