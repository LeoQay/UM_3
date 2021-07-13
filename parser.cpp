
#include <fstream>
#include <cmath>
#include <iostream>

#include "parser.h"
#include "Exception.h"
#include "Tools.h"

using namespace std;

//поменять double на float

void Parser::cellParser (string& s, CommandCode& command, int& op1, int& op2, int& op3)
{
    command = (CommandCode) Tools::StrToInt(s.substr(0, 5));
    op1 = Tools::StrToInt(s.substr(5, 9));
    op2 = Tools::StrToInt(s.substr(14, 9));
    op3 = Tools::StrToInt(s.substr(23, 9));
}


string Parser::getTokenInt()
{
    string token;

    getline(cin, token);

    while (token[0] == ' ') token.erase(0, 1);
    while (token[token.length() - 1] == ' ') token.erase(token[token.length() - 1], 1);

    if (token.length() == 0)
        throw Empty(0, "Empty token!");

    if (!Tools::number(token))
        throw Bad_token(0, token, "Bad int token!");

    string minIntStr = "-2147483648", maxIntStr = "2147483647";

    if (!(Tools::stringCmpGE(token, minIntStr) && Tools::stringCmpGE(maxIntStr, token)))
        throw Bad_token(0, token, "Int Out Range!");

    return token;
}


string Parser::getTokenFloat()
{/*
    float f;
    cin >> f;
    return f;
*/

    string token;

    getline(cin, token);

    while (Tools::CheckSpaceChar(token[0]))
        token.erase(0, 1);

    while (Tools::CheckSpaceChar(token[token.length() - 1]))
        token.erase(token[token.length() - 1], 1);

    if (token.length() == 0)
        throw Empty(0, "Empty token!");

    int commaPos = (int)token.find('.');

    if (commaPos == -1)
    {
        if (!Tools::number(token))
            throw Bad_token(0, token, "Bad float token!");
    }else if (commaPos == token.length() - 1)
    {
        if (!Tools::number(token.substr(0, commaPos)))
            throw Bad_token(0, token, "Bad float token!");
    }else
    {
        if (!Tools::number(token.substr(0, commaPos)) ||
            token[commaPos + 1] == '-' ||
            !Tools::number(token.substr(commaPos + 1, token.length() - commaPos)))
            throw Bad_token(0, token, "Bad float token!");
    }

    return token;
}