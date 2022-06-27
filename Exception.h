#ifndef UM_3_EXCEPTION_H
#define UM_3_EXCEPTION_H

#include <exception>
#include <string>

using namespace std;


class Exception : std::exception
{
protected:
    string message;
    string output_message;
public:
    explicit Exception(string message = "Exception");

    const char* what() noexcept;
};


class TranslatorException : public Exception
{
protected:
    int error_line_number;
public:
    explicit TranslatorException(int    error_line_number,
                        string message           = "Translator Exception");
};


class FieldOutRange : public TranslatorException
{
protected:
    int bad_value;
public:
    FieldOutRange(int    error_line_number,
                  int    bad_value,
                  string message           = "Out range in punched card");
};


class AddressOutRange : public FieldOutRange
{
protected:
    const int low  = 0;
    const int high = 511;
public:
    AddressOutRange(int    error_line_number,
                    int    bad_value,
                    string message           = "Address out range in punched card");
};


class CommandOutRange : public FieldOutRange
{
protected:
    const int low  = 0;
    const int high = 31;
public:
    CommandOutRange(int   error_line_number,
                   int    bad_value,
                   string message           = "Command out range in punched card");
};


class InvalidFieldToken : public TranslatorException
{
protected:
    string field_name;
public:
    InvalidFieldToken(int    error_line_number,
                      string field_name,
                      string message           = "Invalid token in punched card");
};


class EmptyFieldToken : public InvalidFieldToken
{
public:
    EmptyFieldToken(int    error_line_number,
                    string field_name,
                    string message           = "Empty field");
};


class InvalidAddressToken : public InvalidFieldToken
{
protected:
    string invalid_token;
public:
    InvalidAddressToken(int    error_line_number,
                        string invalid_token,
                        string message           = "Syntax error of address");
};


class InvalidCommandToken : public InvalidFieldToken
{
protected:
    string invalid_token;
public:
    InvalidCommandToken(int    error_line_number,
                        string invalid_token,
                        string message           = "Syntax error of command");
};


class InputParserException : public Exception
{
public:
    explicit InputParserException(string message = "Error in input");
};


class InputEmptyToken : public InputParserException
{
public:
    explicit InputEmptyToken(string message = "Empty token in input");
};


class InputInvalidToken : public InputParserException
{
protected:
    string invalid_token;
    string token_type_name;
public:
    explicit InputInvalidToken(string invalid_token,
                               string token_type_name = "",
                               string message         = "Invalid token in input");
};


class InvalidIntToken : public InputInvalidToken
{
public:
    explicit InvalidIntToken(string invalid_token,
                             string message         = "Invalid int token in input");
};


class InvalidFloatToken : public InputInvalidToken
{
public:
    explicit InvalidFloatToken(string invalid_token,
                               string message        = "Invalid float token in input");
};


class RunTimeException : public Exception
{
protected:
    int    error_cell_number;
    string command;
public:
    explicit RunTimeException(int    error_cell_number,
                              string command,
                              string message            = "RunTime Error");
};


class UndefinedCommand : public RunTimeException
{
protected:
    int command_code;
public:
    UndefinedCommand(int    error_cell_number,
                     int    command_code,
                     string command,
                     string message          = "Current command is undefined");
};


class MathException : public RunTimeException
{
public:
    explicit MathException(int    error_cell_number,
                           string command,
                           string message            = "Error in math calculations");
};


class NullDivide : public MathException
{
public:
    explicit NullDivide(int    error_cell_number,
                        string command,
                        string message            = "Null dividing error");
};


class MathOutRange : public MathException
{
public:
    explicit MathOutRange(int    error_cell_number,
                          string command,
                          string message            = "Out Range in math calculations");
};


class FloatToIntOutRange : public MathOutRange
{
protected:
    float value;
public:
    explicit FloatToIntOutRange(int    error_cell_number,
                                string command,
                                float  value,
                                string message            = "Error in conversion FLOAT TO INT");
};


class IntOutRange : public MathOutRange
{
protected:
    int       value1;
    int       value2;
    long long result;
public:
    explicit IntOutRange(int       error_cell_number,
                         string    command,
                         int       value1,
                         int       value2,
                         long long result,
                         string    message           = "Int Out Range");
};


class FloatOutRange : public MathOutRange
{
protected:
    float  value1;
    float  value2;
    double result;
public:
    explicit FloatOutRange(int    error_cell_number,
                           string command,
                           float  value1,
                           float  value2,
                           double result,
                           string message           = "Float Out Range");
};

#endif //UM_3_EXCEPTION_H
