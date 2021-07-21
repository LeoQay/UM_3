#pragma once

#include <string>
#include <map>
#include <set>

using namespace std;

/*
 *  This is an enumeration of all available command names of the UM3 machine
 */
enum CommandCode {
    ININT   = 16,
    OUTINT  = 17,
    ADDINT  = 11,
    SUBINT  = 12,
    MULINT  = 13,
    DIVINT  = 14,
    MOD     = 15,
    INREAL  =  6,
    OUTREAL =  7,
    ADDREAL =  1,
    SUBREAL =  2,
    MULREAL =  3,
    DIVREAL =  4,
    ITOR    = 30,
    RTOI    = 10,
    JUMP    = 9,
    PR      = 20,
    PNR     = 21,
    PB      = 22,
    PM      = 23,
    PBR     = 24,
    PMR     = 25,
    IF      = 19,
    MOV     =  0,
    END     = 31
    // undefined commands: 5, 8, 18, 26, 27, 28, 29
};

class Tools
{
public:
/*
 *  mapCommandTokens - it's a map from STL
 *  which helps to store command tokens
 *  Defined in the constructor
 */
    map <string, CommandCode> mapCommandTokens;

/*
 *  SpaceCharSet - it's a set from STL
 *  which helps to store characters to skip in the program text
 *  Defined in the constructor
 */
    set <char> SpaceCharSet;

    Tools();

/*
 *  This commands used to convert value in cell of memory
 *  between numeric interpretations and string form
 */

    static float IntToFloat (int   value);
    static int   FloatToInt (float value);

    static int StrToInt (string stroka,
                         int    origin_system = 2
                         );

    static string IntToStr (int value,
                            int new_system = 2,
                            int length = 32
                            );

    static float  StrToFloat (string s     );
    static string FloatToStr (float  number);

/*
 *  This function checks token and return true if it is integer token
 */
    static bool number (string s);

/*
 *  return true if integer in s1 >= integer in s2
 *  Examples:
 *  s1      s2      return
 *  "3"     "1"     true
 *  "-12"   "15"    false
 *  "5"     "5"     true
 *  "0"     "-123"  true
 */
    static bool stringCmpGE (string s1,
                             string s2
                             );

/*
 *  return string token of command from mapCommandTokens
 */
    static string getCommandToken (CommandCode command);

/*
 *  return true if character exist in SpaceCharSet
 */
    static bool CheckSpaceChar (char word);

/*
 *  get information about command in cell
 */
    static void ReadCell (std::string   cell,
                          CommandCode & command,
                          int         & op1,
                          int         & op2,
                          int         & op3
                          );

    static int getRandomInt ();

    static string getRightBorderStr (string token,
                                     int    TotalLength,
                                     char   space = ' '
    );
};
