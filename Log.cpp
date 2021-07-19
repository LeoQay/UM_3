#include <string>
#include <sstream>

#include "Log.h"
#include "Tools.h"

using namespace std;

string Log::getTitle()
{
    string token;

    for (int i = 0; i < 5; i++)
        token += Tools::getLeftBorderStr(title_array[i], length_title_array[i]) + " | ";

    return token;
}

string Log::getValueStr(string name, int value)
{
    string answer;

    answer += getName(name) + " | ";

    answer += getInt(value) + " | ";

    answer += getReal(value) + " | ";

    answer += getString(value) + " | ";

    answer += getCommand(value) + " |";

    return answer;
}

string Log::getName(string name)
{
    return Tools::getLeftBorderStr(std::move(name), length_title_array[0]);
}

string Log::getInt(int value)
{
    return Tools::getLeftBorderStr(Tools::IntToStr(value, 10, 0), length_title_array[1]);
}

string Log::getReal(int value)
{
    ostringstream help;
    help << Tools::IntToFloat(value);
    return Tools::getLeftBorderStr(help.str(), length_title_array[2]);
}

string Log::getString(int value)
{
    return Tools::getLeftBorderStr("\"" + Tools::IntToStr(value) + "\"", length_title_array[3]);
}

string Log::getCommand(int value)
{
    CommandCode command;
    int op1, op2, op3;
    Tools::ReadCell(Tools::IntToStr(value), command, op1, op2, op3);
    string token = Tools::getLeftBorderStr(Tools::getCommandToken(command), 6) + "|"
                   + Tools::getLeftBorderStr(Tools::IntToStr(op1, 10, 0), 3, '0') + "|"
                   + Tools::getLeftBorderStr(Tools::IntToStr(op2, 10, 0), 3, '0') + "|"
                   + Tools::getLeftBorderStr(Tools::IntToStr(op3, 10, 0), 3, '0');

    return Tools::getLeftBorderStr(token, length_title_array[4]);
}
