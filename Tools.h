#pragma once

#include <string>
#include <map>
#include <set>

using namespace std;

enum CommandCode {
    ININT = 16,
    OUTINT = 17,
    ADDINT = 11,
    SUBINT = 12,
    MULINT = 13,
    DIVINT = 14,
    MOD = 15,
    INREAL = 6,
    OUTREAL = 7,
    ADDREAL = 1,
    SUBREAL = 2,
    MULREAL = 3,
    DIVREAL = 4,
    ITOR = 30,
    RTOI = 10,
    JUMP = 9,
    PR = 20,
    PNR = 21,
    PB = 22,
    PM = 23,
    PBR = 24,
    PMR = 25,
    IF = 19,
    MOV = 0,
    END = 31
    // undefined commands: 5, 8, 18, 26, 27, 28, 29
    // enjoy for fantasy
};

class Tools
{
public:
    map <string, CommandCode> mapCommands;

    set <char> SpaceCharSet;

    Tools();

    static float IntToFloat (int value);
    static int FloatToInt (float value);
    static int StrToInt (string stroka, int origin_system = 2);
    static string IntToStr (int value, int length = 32, int new_system = 2);
    static float StrToFloat (string s);
    static string FloatToStr (float number);

    static bool number (string s); // true, если в строке целое число

    static bool stringCmpGE (string s1, string s2);   // compare, stringCmpGE("-2", "3") = false

    static string getCommandLexem(CommandCode command);

    static bool CheckSpaceChar (char word);
};
