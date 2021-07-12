#include <string>
#include <fstream>

#include "Translator.h"
#include "Tools.h"
#include "Exception.h"

using namespace std;

int Translator::command_check (std::string command, int num)
{
    if (Tools::number(command))
    {
        int answer = Tools::stoi(command, 10);
        if (answer >= 0 && answer <= 31)
            return answer;
        else
            throw IndexOutRange(num, answer, "Out range of command!");
    }

    // сюда попадем только, если команда - не число
    // mapCommands - map, хранящий список доступных команд

    Tools tools;

    if ((tools.mapCommands).find(command) == (tools.mapCommands).end())
        throw Bad_token(num, command, "Bad token of command!");

    return (tools.mapCommands)[command];
}

std::string Translator::getTokenCell (std::string& token, int num)
{
    // данная функция отрезает от входной строки первый токен
    // обособленный пробелами или концами строки
    // если строка пуста или состоит из пробелов, бросается исключение

    std::string answer;

    int iter = 0;
    while ((token[iter] == ' ' || token[iter] == '\t') && iter < token.length()) iter++;

    if (iter == token.length()) throw Empty(num, "Empty");

    token.erase(0, iter);

    iter = 0;
    while (iter < token.size() && (token[iter] != ' ' && token[iter] != '\t')) answer += token[iter++];
    token.erase(0, iter);

    return answer;
}

void Translator::Translate (string PunchedCard_file_name, Memory& mem_obj)
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
        if (!Tools::number(token))
        {
            fin.close();
            throw Bad_token(cellNumber, token, "Bad token of cell number!");
        }

        // все-таки числовая, проверим на выход за диапазон
        position = Tools::stoi(token, 10);

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
        result += Tools::itos(command_check(token, cellNumber), 5);

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
            if (!Tools::number(token))
            {
                fin.close();
                throw Bad_token(cellNumber, token, "Bad token of " + opi + "!");
            }

            int token_val = Tools::stoi(token, 10);

            if (token_val > 511 || token_val < 0)
            {
                fin.close();
                throw IndexOutRange(cellNumber, token_val, "Index out range " + opi + "!");
            }

            // каждому операнду отводится по 9 бит
            result += Tools::itos(token_val, 9);
        }

        mem_obj.push(position, result);
    }

    fin.close();
}