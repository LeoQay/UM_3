#pragma once

#include <sstream>

#include "Exception.h"


Exception::Exception (int num, std::string message) : message(move(message)), cell_number(num) {};

const char* Exception::what() noexcept
{
    return message.c_str();
}

ParserException::ParserException (int num, std::string message) : Exception(num, move(message)) {};

Empty::Empty(int num, std::string message) : ParserException(num, move(message)) {};

IndexOutRange::IndexOutRange(int num, int bad_index, std::string message) : ParserException(num, move(message))
{
    std::ostringstream oss;
    oss << bad_index;
    this->message += (" " + oss.str());
}

Bad_token::Bad_token(int num, std::string& token, std::string message) : ParserException(num, move(message))
{
    this->message += (" " + token);
}

ProcessorException::ProcessorException (int num,int err_com, int op1, int op2, int op3, std::string message) :
        Exception(num, move(message))
{
    std::ostringstream oss;
    oss << "\nCell: " << err_com << " " << op1 << " " << op2 << " " << op3;
    this->message += oss.str();
}

Bad_command::Bad_command (int num, int err_com, int op1, int op2, int op3, std::string message) :
        ProcessorException(num, err_com, op1, op2, op3, move(message)) {};

MathException::MathException (int num, int err_com, int op1, int op2, int op3, std::string message) :
        ProcessorException(num, err_com, op1, op2, op3, move(message)) {};

MathException::MathException (int num, int err_com, int op1, int op2, int op3,
                              int iop2, int iop3, std::string message) :
        ProcessorException(num, err_com, op1, op2, op3, move(message))
{
    std::ostringstream oss;
    oss << "\nint: op2 = " << iop2 << ", op3 = " << iop3;
    message += oss.str();
}

MathException::MathException (int num, int err_com, int op1, int op2, int op3,
                              float fop2, float fop3, std::string message) :
        ProcessorException(num, err_com, op1, op2, op3, move(message))
{
    std::ostringstream oss;
    oss << "\nfloat: op2 = " << fop2 << ", op3 = " << fop3;
    message += oss.str();
}

NULL_DIVIDE::NULL_DIVIDE (int num, int err_com, int op1, int op2, int op3, std::string message) :
        MathException(num, err_com, op1, op2, op3, move(message)) {};

MathOutRange::MathOutRange (int num, int err_com, int op1, int op2, int op3, std::string message) :
        MathException(num, err_com, op1, op2, op3, move(message)) {};

MathOutRange::MathOutRange (int num, int err_com, int op1, int op2, int op3,
                            int iop2, int iop3, std::string message) :
        MathException(num, err_com, op1, op2, op3, iop2, iop3, move(message)) {};

MathOutRange::MathOutRange (int num, int err_com, int op1, int op2, int op3,
                            float fop2, float fop3, std::string message) :
        MathException(num, err_com, op1, op2, op3, fop2, fop3, move(message)) {};

FTOIOutRange::FTOIOutRange (int num, int err_com, int op1, int op2, int op3, long double res, std::string message) :
        MathOutRange(num, err_com, op1, op2, op3, move(message))
{
    std::ostringstream oss;
    oss << "\n" << "Bad value: " << res;
    this->message += oss.str();
}

MemoryException::MemoryException (int num, int address, std::string message) : Exception(num, move(message))
{
    std::ostringstream oss;
    oss << "\nAddress: " << address;
    this->message += oss.str();
}

MemoryUndefined::MemoryUndefined (int num, int address, std::string message) :
        MemoryException(num, address, move(message))
{
    this->address = address;
}