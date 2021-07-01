#pragma once

#include <exception>
#include <string>

class Exception : std::exception
{
protected:
    std::string message;
public:
    int cell_number;

    explicit Exception (int num, std::string message = "Error!");
    const char* what() noexcept;
};

class ParserException : public Exception
{
public:
    explicit ParserException (int num, std::string message = "Bad cell!");
};

class Empty : public ParserException
{
public:
    explicit Empty(int num, std::string message = "Empty!");
};

class IndexOutRange : public ParserException
{
public:
    IndexOutRange(int num, int bad_index, std::string message = "Out range!");
};

class Bad_token : public ParserException
{
public:
    explicit Bad_token(int num, std::string& token, std::string message = "Bad token!");
};

class ProcessorException : public Exception
{
public:
    ProcessorException (int num,int err_com, int op1, int op2, int op3, std::string message = "Runtime error!");
};

class Bad_command : public ProcessorException
{
public:
    Bad_command (int num, int err_com, int op1, int op2, int op3, std::string message = "Bad command!");
};

class MathException : public ProcessorException
{
public:
    MathException (int num, int err_com, int op1, int op2, int op3, std::string message = "Math error!");

    MathException (int num, int err_com, int op1, int op2, int op3,
                   int iop2, int iop3, std::string message = "Math error!");

    MathException (int num, int err_com, int op1, int op2, int op3,
                   float fop2, float fop3, std::string message = "Math error!");
};

class NULL_DIVIDE : public MathException
{
public:
    NULL_DIVIDE (int num, int err_com, int op1, int op2, int op3, std::string message = "Dividing by zero is bad!");
};

class MathOutRange : public MathException
{
public:
    // простой ответ
    MathOutRange (int num, int err_com, int op1, int op2, int op3, std::string message = "Math out range!");

    // ответ для операций с целыми
    MathOutRange (int num, int err_com, int op1, int op2, int op3,
                  int iop2, int iop3, std::string message = "Math out range!");

    // ответ для операций с вещественными
    MathOutRange (int num, int err_com, int op1, int op2, int op3,
                  float fop2, float fop3, std::string message = "Math out range!") ;
};

class FTOIOutRange : public MathOutRange
{
public:
    FTOIOutRange (int num, int err_com, int op1, int op2, int op3, long double res,
                  std::string message = "Float to int out range!");
};

class MemoryException : public Exception
{
public:
    MemoryException (int num, int address, std::string message = "Memory exception!");
};

class MemoryUndefined : public MemoryException
{
public:
    int address;
    MemoryUndefined (int num, int address, std::string message = "Accessing Undefined Memory!");
};
