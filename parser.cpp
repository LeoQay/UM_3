
#include <fstream>
#include <cmath>

#include "parser.h"
#include "Exception.h"

using namespace std;

int Parser::command_check (string command, int num)
{
    if (number(command))
    {
        int answer = stoi(command, 10);
        if (answer >= 0 && answer <= 31)
            return answer;
        else
            throw IndexOutRange(num, answer, "Out range of command!");
    }

    // сюда попадем только, если команда - не число
    // mapCommands - map, хранящий список доступных команд

    if (mapCommands.find(command) == mapCommands.end())
        throw Bad_token(num, command, "Bad token of command!");

    return mapCommands[command];
}

string Parser::getTokenCell (string& token, int num)
{
    // данная функция отрезает от входной строки первый токен
    // обособленный пробелами или концами строки
    // если строка пуста или состоит из пробелов, бросается исключение

    string answer;

    int iter = 0;
    while (token[iter] == ' ' && iter < token.length()) iter++;

    if (iter == token.length()) throw Empty(num, "Empty");

    token.erase(0, iter);

    iter = 0;
    while (iter < token.size() && token[iter] != ' ') answer += token[iter++];
    token.erase(0, iter);

    return answer;
}

//поменять double на float

void Parser::get_punched_card (string PunchedCard_file_name, Memory& mem_obj)
{
    // функция парсит перфокарту и бросает исключения, если допущены ошибки

    ifstream fin;

    fin.open(PunchedCard_file_name);

    int cellNumber = 0;

    while (!fin.eof())
    {
        cellNumber++;

        int position;
        string cell, result, token;

        getline(fin, cell);

        // удаление комментариев
        unsigned int comment_pos = cell.find(';');
        if (comment_pos != -1)
            cell.erase(comment_pos, cell.length() - comment_pos);

        //  парсинг номера ячейки
        try{
            token = getTokenCell(cell, cellNumber);
        }
        catch (Empty&)
        {
            continue;
        }

        // проверка на то, является ли прочитанная лексема числовой
        if (!number(token))
        {
            fin.close();
            throw Bad_token(cellNumber, token, "Bad token of cell number!");
        }

        // все-таки числовая, проверим на выход за диапазон
        position = stoi(token, 10);

        if (position > 511 || position < 0)
        {
            fin.close();
            throw IndexOutRange(cellNumber, position, "Cell number out range!");
        }

        //   команда
        try {
            token = getTokenCell(cell, cellNumber);
        }

        catch (Empty&)
        {
            fin.close();
            throw Empty(cellNumber, "Empty command!");
        }

        // командам в памяти ум3 отводится 5 битов
        result += itos(command_check(token, cellNumber), 5);

        // op1, op2, op3
        for (int i = 0; i < 3; i++)
        {
            string opi = "op1"; opi[2] += i;

            try{
                token = getTokenCell(cell, cellNumber);
            }

            catch (Empty&)
            {
                fin.close();
                throw Empty(cellNumber, "Empty " + opi + "!");
            }

            // адреса операндов должны быть числами
            if (!number(token))
            {
                fin.close();
                throw Bad_token(cellNumber, token, "Bad token of " + opi + "!");
            }

            int token_val = stoi(token, 10);

            if (token_val > 511 || token_val < 0)
            {
                fin.close();
                throw IndexOutRange(cellNumber, token_val, "Index out range " + opi + "!");
            }

            // каждому операнду отводится по 9 бит
            result += itos(token_val, 9);
        }

        mem_obj.push(position, result);
    }

    fin.close();
}

bool Parser::number(string s)  // проверка на числовую лексему
{
    int i = 0;
    while (((i == 0 && s[0] == '-') || isdigit(s[i])) && i < s.length()) i++;

    return i == s.length();
}

void Parser::cellParser (string& s, CommandCode& command, int& op1, int& op2, int& op3)
{
    command = (CommandCode)stoi(s.substr(0, 5));
    op1 = stoi(s.substr(5, 9));
    op2 = stoi(s.substr(14, 9));
    op3 = stoi(s.substr(23, 9));
}

int Parser::stoi (std::string stroka, int origin_system)
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

std::string Parser::itos (int value, int length, int new_system)
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

long double Parser::stold (string s)
{
    // в памяти +0 = 000...00,  -0 = 100...00
    if (s.substr(1, 31) == "0000000000000000000000000000000")  // 31 zero
        return 0;

    int sign = 1 - 2 * (s[0] - '0');     // при (int)s[0] = 48 sign = 1, а при 49 sign = -1
    int E = stoi(s.substr(1, 8), 2);
    int sub_Mantis = stoi('1' + s.substr(9, 23), 2);

    return sign * ((long double)sub_Mantis * pow(2, E - 127 - 23));
}

string Parser::ftos(float number) /* вещественное число 1.Mantis * (2 ^ (E - 127)) */
{
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

    string answer = sign + itos(E, 8) + itos(Mantis, 23);

    return answer;
}

string Parser::getTokenInt()
{
    string token;
    char c = '-';
    while (c != '\n')
    {
        c = (char)getchar();
        if (c != '\n') token += c;
    }

    while (token[0] == ' ') token.erase(0, 1);
    while (token[token.length() - 1] == ' ') token.erase(token[token.length() - 1], 1);

    if (token.length() == 0)
        throw Empty(0, "Empty token!");

    if (!number(token))
        throw Bad_token(0, token, "Bad int token!");

    string minIntStr = "-2147483648", maxIntStr = "2147483647";

    if (!(stringCmpGE(token, minIntStr) && stringCmpGE(maxIntStr, token)))
        throw Bad_token(0, token, "Int Out Range!");

    return token;
}

// GRADE EQUAL
bool Parser::stringCmpGE (string s1, string s2)      // return s1 >= s2 ? true : false
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

float Parser::getTokenFloat()
{
    string token;
    char c = '-';

    while (c != '\n')
    {
        c = (char)getchar();
        if (c != '\n') token += c;
    }

    while (token[0] == ' ') token.erase(0, 1);
    while (token[token.length() - 1] == ' ') token.erase(token[token.length() - 1], 1);

    if (token.length() == 0)
        throw Empty(0, "Empty token!");

    int commaPos = (int)token.find('.');

    if (commaPos == -1)
    {
        if (!number(token))
            throw Bad_token(0, token, "Bad float token!");
    }else if (commaPos == token.length() - 1)
    {
        if (!number(token.substr(0, commaPos)))
            throw Bad_token(0, token, "Bad float token!");
    }else
    {
        if (!number(token.substr(0, commaPos)) ||
            token[commaPos + 1] == '-' ||
            !number(token.substr(commaPos + 1, token.length() - commaPos)))
            throw Bad_token(0, token, "Bad float token!");
    }

    return (float)strtod(token.c_str(), nullptr);
}

string Parser::getCommandLexem(CommandCode command)
{
    for (auto & var : mapCommands)
        if (var.second == command)
            return var.first;
    return "";
}