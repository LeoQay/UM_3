#include <string>
#include <sstream>

#include "Log.h"
#include "Tools.h"

using namespace std;

string Log::getTitle()
{
    Log log;
    string token;

    for (int i = 0; i < 5; i++)
        token += Tools::getRightBorderStr(log.title_array[i], log.length_title_array[i]) + " | ";

    return token;
}

string Log::getValueStr(string name, int value)
{
    Log log;
    string answer;

    answer += log.getName(name) + " | ";

    answer += log.getInt(value) + " | ";

    answer += log.getReal(value) + " | ";

    answer += log.getString(value) + " | ";

    answer += log.getCommand(value) + " |";

    return answer;
}

string Log::getName(string name)
{
    return Tools::getRightBorderStr(std::move(name), length_title_array[0]);
}

string Log::getInt(int value)
{
    return Tools::getRightBorderStr(Tools::IntToStr(value, 10, 0), length_title_array[1]);
}

string Log::getReal(int value)
{
    ostringstream help;
    help << Tools::IntToFloat(value);
    return Tools::getRightBorderStr(help.str(), length_title_array[2]);
}

string Log::getString(int value)
{
    return Tools::getRightBorderStr("\"" + Tools::IntToStr(value) + "\"", length_title_array[3]);
}

string Log::getCommand(int value)
{
    CommandCode command;
    int op1, op2, op3;
    Tools::ReadCell(Tools::IntToStr(value), command, op1, op2, op3);

    string token = Tools::getRightBorderStr(Tools::getCommandToken(command), 6) + "|"
                   + Tools::getRightBorderStr(Tools::IntToStr(op1, 10, 0), 3, '0') + "|"
                   + Tools::getRightBorderStr(Tools::IntToStr(op2, 10, 0), 3, '0') + "|"
                   + Tools::getRightBorderStr(Tools::IntToStr(op3, 10, 0), 3, '0');

    return Tools::getRightBorderStr(token, length_title_array[4]);
}

string Log::BinaryOperator(string RK, int addressOP2, int addressOP3, int valueOP2, int valueOP3)
{
    string answer;

    string nameOP2 = getAddressToken(addressOP2);
    string nameOP3 = getAddressToken(addressOP3);

    answer += getTitle() + "\n"
            + getValueStr(nameOP2, valueOP2) + "\n"
            + getValueStr(nameOP3, valueOP3);

    return answer;
}

string Log::getAddressToken(int address)
{
    return "<" + Tools::IntToStr(address, 10, 0) + ">";
}

string Log::getNiceCommand(int address, string RK)
{
    string title = "ADDRESS | COD | COMMAND | OP1 | OP2 | OP3";

    CommandCode command;
    int op1, op2, op3;
    Tools::ReadCell(RK, command, op1, op2, op3);

    string answer = title
                  + "\n"
                  + Tools::getRightBorderStr(Tools::getRightBorderStr(Tools::IntToStr(address, 10, 0), 3, '0'), 7)
                  + " | "
                  + Tools::getRightBorderStr(Tools::IntToStr(command, 10, 0), 3)
                  + " | "
                  + Tools::getRightBorderStr(Tools::getCommandToken(command), 7)
                  + " | "
                  + Tools::getRightBorderStr(Tools::IntToStr(op1, 10, 0), 3, '0')
                  + " | "
                  + Tools::getRightBorderStr(Tools::IntToStr(op2, 10, 0), 3, '0')
                  + " | "
                  + Tools::getRightBorderStr(Tools::IntToStr(op3, 10, 0), 3, '0');

    return answer;
}
