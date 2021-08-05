#pragma once

#include <sstream>

#include "Exception.h"
#include "Log.h"

using namespace std;


Exception::Exception(string message) : message(message)
{
    output_message = message;
}


const char *Exception::what() noexcept
{
    return output_message.c_str();
}


TranslatorException::TranslatorException(int    error_line_number,
                                         string message)
                                         :
                                         Exception(message),
                                         error_line_number(error_line_number)
{
    ostringstream buf;
    buf << error_line_number;
    string number_token = buf.str();

    output_message += "\n";
    output_message += "Error in the line ";
    output_message += number_token;
}


FieldOutRange::FieldOutRange(int    error_line_number,
                             int    bad_value,
                             string message)
                             :
                             TranslatorException(error_line_number, message),
                             bad_value(bad_value)
{
    ostringstream buf;
    buf << bad_value;
    string number_token = buf.str();

    output_message += "\n";
    output_message += "Bad value -- ";
    output_message += number_token;
}


AddressOutRange::AddressOutRange(int    error_line_number,
                                 int    bad_value,
                                 string message)
                             :
                             FieldOutRange(error_line_number, bad_value, message)
{
    ostringstream buf;
    buf << low;
    string number_token_low = buf.str();
    buf.clear();
    buf << high;
    string number_token_high = buf.str();

    output_message += "\n";
    output_message += "Distance is ";
    output_message += "[" + number_token_low + ", " + number_token_high + "]";
}


CommandOutRange::CommandOutRange(int    error_line_number,
                                 int    bad_value,
                                 string message)
                                 :
                                 FieldOutRange(error_line_number, bad_value, message)
{
    ostringstream buf;
    buf << low;
    string number_token_low = buf.str();
    buf.clear();
    buf << high;
    string number_token_high = buf.str();

    output_message += "\n";
    output_message += "Distance is ";
    output_message += "[" + number_token_low + ", " + number_token_high + "]";
}


InvalidFieldToken::InvalidFieldToken(int    error_line_number,
                                     string field_name,
                                     string message)
                                     :
                                     TranslatorException(error_line_number, message)
{
    output_message += "\n";
    output_message += "Invalid token of ";
    output_message += field_name;
}


EmptyFieldToken::EmptyFieldToken(int    error_line_number,
                                 string field_name,
                                 string message)
                                 :
                                 InvalidFieldToken(error_line_number,field_name, message)
{
    int buf = output_message.rfind('\n');
    output_message.erase(buf + 1);

    output_message += "Empty token of ";
    output_message += field_name;
}


InvalidAddressToken::InvalidAddressToken(int    error_line_number,
                                         string invalid_token,
                                         string message)
                                         :
                                         InvalidFieldToken(error_line_number, "Address", message),
                                         invalid_token(invalid_token)
{
    output_message += " -- ";
    output_message += invalid_token;
}


InvalidCommandToken::InvalidCommandToken(int    error_line_number,
                                         string invalid_token,
                                         string message)
                                         :
                                         InvalidFieldToken(error_line_number, "Command", message)
{
    output_message += " -- ";
    output_message += invalid_token;
}


InputParserException::InputParserException(string message)
                                           :
                                           Exception(message)
                                           {}


InputEmptyToken::InputEmptyToken(string message)
                                 :
                                 InputParserException(message)
                                 {}


InputInvalidToken::InputInvalidToken(string invalid_token,
                                     string token_type_name,
                                     string message)
                                     :
                                     InputParserException(message)
{
    output_message += "Invalid token";

    if(token_type_name != "")
    {
        output_message += " of ";
        output_message += token_type_name;
    }

    output_message += " -- ";
    output_message += invalid_token;
}


InvalidIntToken::InvalidIntToken(string invalid_token,
                                 string message)
                                 :
                                 InputInvalidToken(invalid_token, "INT", message)
                                 {}


InvalidFloatToken::InvalidFloatToken(string invalid_token,
                                     string message)
                                     :
                                     InputInvalidToken(invalid_token, "FLOAT", message)
                                     {}


RunTimeException::RunTimeException(int    error_cell_number,
                                   string command,
                                   string message)
                                   :
                                   Exception(message),
                                   error_cell_number(error_cell_number),
                                   command(command)
{
    output_message += "\n";
    output_message += "Last command";
    output_message += "\n";
    output_message += Log::getNiceCommand(error_cell_number, command);
}

UndefinedCommand::UndefinedCommand(int    error_cell_number,
                                   int    command_code,
                                   string command,
                                   string message)
                                   :
                                   RunTimeException(error_cell_number,command, message),
                                   command_code(command_code)
{
    ostringstream buf;
    buf << command_code;
    string number_token = buf.str();

    output_message += "\n";
    output_message += "Code of undefined command -- ";
    output_message += number_token;
}


MathException::MathException(int    error_cell_number,
                             string command,
                             string message)
                             : RunTimeException(error_cell_number,command, message)
                             {}


NullDivide::NullDivide(int    error_cell_number,
                       string command,
                       string message)
                       :
                       MathException(error_cell_number,command, message)
                       {}


MathOutRange::MathOutRange(int    error_cell_number,
                           string command,
                           string message)
                           :
                           MathException(error_cell_number,command, message)
                           {}


FloatToIntOutRange::FloatToIntOutRange(int    error_cell_number,
                                       string command,
                                       float  value,
                                       string message)
                                       :
                                       MathOutRange(error_cell_number, command, message),
                                       value(value)
{
    ostringstream buf;
    buf << value;
    string number_token = buf.str();

    output_message += "\n";
    output_message += "FLOAT value -- ";
    output_message += number_token;
}


IntOutRange::IntOutRange(int       error_cell_number,
                         string    command,
                         int       value1,
                         int       value2,
                         long long result,
                         string    message)
                         :
                         MathOutRange(error_cell_number, command, message),
                         value1(value1),
                         value2(value2),
                         result(result)
{
    ostringstream buf1, buf2, buf_res;
    buf1    << value1;
    buf2    << value2;
    buf_res << result;

    output_message += "\n";
    output_message += "Value1 = ";
    output_message += buf1.str();
    output_message += "\n";
    output_message += "Value2 = ";
    output_message += buf2.str();
    output_message += "\n";
    output_message += "Result = ";
    output_message += buf_res.str();
}


FloatOutRange::FloatOutRange(int   error_cell_number,
                            string command,
                            float  value1,
                            float  value2,
                            double result,
                            string message)
                            :
                            MathOutRange(error_cell_number, command, message),
                            value1(value1),
                            value2(value2),
                            result(result)
{
    ostringstream buf1, buf2, buf_res;
    buf1    << value1;
    buf2    << value2;
    buf_res << result;

    output_message += "\n";
    output_message += "Value1 = ";
    output_message += buf1.str();
    output_message += "\n";
    output_message += "Value2 = ";
    output_message += buf2.str();
    output_message += "\n";
    output_message += "Result = ";
    output_message += buf_res.str();
}
