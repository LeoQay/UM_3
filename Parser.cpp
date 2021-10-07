#include <iostream>

#include "Parser.h"
#include "Exception.h"
#include "Tools.h"

using namespace std;

string Parser::getTokenInt()
{
    string token;

    getline(cin, token);

    while (Tools::CheckSpaceChar(token[0])) token.erase(0, 1);
    while (Tools::CheckSpaceChar(token[token.length() - 1])) token.erase(token[token.length() - 1], 1);

    if (token.length() == 0)
        throw InputEmptyToken();

    if (!Tools::number(token))
        throw InvalidIntToken(token);

    string minIntStr = "-2147483648", maxIntStr = "2147483647";

    if (!(Tools::stringCmpGE(token, minIntStr) && Tools::stringCmpGE(maxIntStr, token)))
        throw InvalidIntToken(token, "Int Out Range!");

    return token;
}


string Parser::getTokenFloat()
{
    string token;
    getline(cin, token);

    while (Tools::CheckSpaceChar(token[0]))
        token.erase(0, 1);

    while (Tools::CheckSpaceChar(token[token.length() - 1]))
        token.erase(token[token.length() - 1], 1);

    if (token.length() == 0)
        throw InputEmptyToken();

    int commaPos = (int)token.find('.');

    if (commaPos == -1)
    {
        if (!Tools::number(token))
            throw InvalidFloatToken(token);
    }else if (commaPos == token.length() - 1)
    {
        if (!Tools::number(token.substr(0, commaPos)))
            throw InvalidFloatToken(token);
    }else
    {
        if (!Tools::number(token.substr(0, commaPos)) ||
            token[commaPos + 1] == '-' ||
            !Tools::number(token.substr(commaPos + 1, token.length() - commaPos)))
            throw InvalidFloatToken(token);
    }

    return token;
}