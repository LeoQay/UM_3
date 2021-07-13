#include <string>
#include <map>
#include <cmath>

#include "Tools.h"

using namespace std;

Tools::Tools()
{
    mapCommands = {
            {"ININT",    ININT   },
            {"OUTINT",   OUTINT  },
            {"ADDINT",   ADDINT  },
            {"SUBINT",   SUBINT  },
            {"MULINT",   MULINT  },
            {"DIVINT",   DIVINT  },
            {"MOD",      MOD     },
            {"INREAL",   INREAL  },
            {"OUTREAL",  OUTREAL },
            {"ADDREAL",  ADDREAL },
            {"SUBREAL",  SUBREAL },
            {"MULREAL",  MULREAL },
            {"DIVREAL",  DIVREAL },
            {"ITOR",     ITOR    },
            {"RTOI",     RTOI    },
            {"JUMP",     JUMP    },
            {"PR",       PR      },
            {"PNR",      PNR     },
            {"PB",       PB      },
            {"PM",       PM      },
            {"PBR",      PBR     },
            {"PMR",      PMR     },
            {"MOV",      MOV     },
            {"IF",       IF      },
            {"END",      END     }
    };

    setSpaceChar = {
            ' ',
            '\t'
    };
}


bool Tools::number(std::string s)  // проверка на числовую лексему
{
    int i = 0;
    while (((i == 0 && s[0] == '-') || isdigit(s[i])) && i < s.length()) i++;

    return i == s.length();
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


std::string Tools::IntToStr (int value, int length, int new_system)
{
    auto val = (unsigned int)value;
    string answer;

    for (int i = 0; i < length; i++)
    {
        answer.insert(0, 1, (char)('0' + val % new_system));
        val /= new_system;
    }

    return answer;
}


float Tools::StrToFloat (string s)
{

    int value = Tools::StrToInt(s);

    return *((float*)(&value));

    /*

    // в памяти +0 = 000...00,  -0 = 100...00

    if (s.substr(1, 31) == "0000000000000000000000000000000")  // 31 zero
        return 0;

    int sign = 1 - 2 * (s[0] - '0');     // при (int)s[0] = 48 sign = 1, а при 49 sign = -1
    int E = stoi(s.substr(1, 8), 2);
    int sub_Mantis = StrToInt('1' + s.substr(9, 23), 2);

    return sign * ((long double)sub_Mantis * pow(2, E - 127 - 23));

    */
}


string Tools::FloatToStr(float number) /* вещественное число 1.Mantis * (2 ^ (E - 127)) */
{

    int* val = (int*)&number;

    int p = *val;

    return Tools::IntToStr(*((int*)(&number)));

    /*

    if (number == 0) return "00000000000000000000000000000000"; // 32 zero

    char sign = number > 0 ? '0' : '1';
    float positive = abs(number);
    int degree = 0;

    // приведение к виду 1.Mantis * 2^degree
    while (positive / 2 >= 1)
    {
        positive /= 2;
        degree++;
    }
    while (positive < 1)
    {
        positive *= 2;
        degree--;
    }

    int E = 127 + degree;

    positive -= 1;  // приведение к виду 0.Mantis
    int Mantis = (int)(positive * pow(2, 23)); // приведение к виду Mantis

    string answer = sign + itos(E, 8) + IntToStr(Mantis, 23);

    return answer;

    */
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


string Tools::getCommandLexem(CommandCode command)
{
    Tools tools;

    for (auto & var : tools.mapCommands)
        if (var.second == command)
            return var.first;
    return "";
}

bool Tools::CheckSpaceChar(char word)
{
    Tools tools;

    if (tools.setSpaceChar.find(word) == tools.setSpaceChar.end())
        return false;
    else
        return true;
}
