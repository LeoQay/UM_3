#include <string>
#include <map>
#include <cmath>
#include <random>
#include <ctime>

#include "Tools.h"

using namespace std;

Tools::Tools()
{
    if (mapCommandTokens.empty())
    {
        mapCommandTokens = {
                {"ININT",   ININT},
                {"OUTINT",  OUTINT},
                {"ADDINT",  ADDINT},
                {"SUBINT",  SUBINT},
                {"MULINT",  MULINT},
                {"DIVINT",  DIVINT},
                {"MOD",     MOD},
                {"INREAL",  INREAL},
                {"OUTREAL", OUTREAL},
                {"ADDREAL", ADDREAL},
                {"SUBREAL", SUBREAL},
                {"MULREAL", MULREAL},
                {"DIVREAL", DIVREAL},
                {"ITOR",    ITOR},
                {"RTOI",    RTOI},
                {"JUMP",    JUMP},
                {"PR",      PR},
                {"PNR",     PNR},
                {"PB",      PB},
                {"PM",      PM},
                {"PBR",     PBR},
                {"PMR",     PMR},
                {"MOV",     MOV},
                {"IF",      IF},
                {"END",     END}
        };
    }

    if (SpaceCharSet.empty())
    {
        SpaceCharSet = {
                ' ',
                '\t'
        };
    }
}


bool Tools::number(std::string s)  // проверка на числовую лексему
{
    int i = 0;
    while (((i == 0 && s[0] == '-') || isdigit(s[i])) && i < s.length()) i++;

    return i == s.length();
}

float Tools::IntToFloat(int value)
{
    return *(float*)&value;
}

int Tools::FloatToInt(float value)
{
    return *(int*)&value;
}

int Tools::StrToInt (std::string stroka, int origin_system)
{
    int num = 0, ten_sys, it = 0;
    int sign = 1;

    if (stroka[0] == '-')
    {
        sign = -1;
        stroka.erase(0, 1);
    }

    while (it < stroka.size())
    {
        ten_sys = (int)stroka[it] - '0';
        num = origin_system * num + ten_sys;
        it++;
    }

    return num * sign;
}


std::string Tools::IntToStr (int value, int new_system, int length)
{
    auto val = (unsigned int)value;
    string answer;

    while (val != 0 || answer.length() < length)
    {
        answer.insert(0, 1, (char)('0' + val % new_system));
        val /= new_system;
    }

    return answer;
}


float Tools::StrToFloat (string s)
{
    return IntToFloat(Tools::StrToInt(s));
}


string Tools::FloatToStr(float number) /* вещественное число 1.Mantis * (2 ^ (E - 127)) */
{
    return Tools::IntToStr(FloatToInt(number));
}


// GRADE EQUAL
bool Tools::stringCmpGE (string s1, string s2)      // return s1 >= s2 ? true : false
{
    if (s1[0] != '-' && s2[0] == '-') return true;
    if (s1[0] == '-' && s2[0] != '-') return false;
    bool reverse = false;

    if (s1[0] == '-' && s2[0] == '-')
    {
        reverse = true;
        s1.erase(0, 1);
        s2.erase(0, 1);
    }

    if (s1.length() > s2.length()) return !reverse;
    if (s1.length() < s2.length()) return reverse;

    if (s1 >= s2)
        return !reverse;
    else
        return reverse;
}


string Tools::getCommandToken(CommandCode command)
{
    Tools tools;

    for (auto & var : tools.mapCommandTokens)
        if (var.second == command)
            return var.first;
    return Tools::IntToStr((int)command, 10, 0);
}

bool Tools::CheckSpaceChar(char word)
{
    Tools tools;

    return tools.SpaceCharSet.find(word) != tools.SpaceCharSet.end();
}

void Tools::ReadCell (string cell, CommandCode& command, int& op1, int& op2, int& op3)
{
    command = (CommandCode) Tools::StrToInt(cell.substr(0, 5));
    op1 = Tools::StrToInt(cell.substr(5, 9));
    op2 = Tools::StrToInt(cell.substr(14, 9));
    op3 = Tools::StrToInt(cell.substr(23, 9));
}

int Tools::getRandomInt()
{
    static long long seed = time(nullptr);

    std::mt19937 gen(seed);

    std::uniform_int_distribution<> dist(-2147483648, 2147483647);

    dist(gen);

    seed = dist(gen);

    // сброс плохого числа, испорченного из-за time

    return dist(gen);
}

string Tools::getLeftBorderStr(string token, int TotalLength, char space)
{
    int kol = TotalLength - token.length();

    for (int i = 0; i < kol; i++)
        token = space + token;

    return token;
}



