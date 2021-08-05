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
        int answer = Tools::StrToInt(command, 10);
        if (answer >= 0 && answer <= 31)
            return answer;
        else
            throw CommandOutRange(num, answer);
    }

    // сюда попадем только, если команда - не число
    // mapCommands - map, хранящий список доступных команд

    Tools tools;

    if ((tools.mapCommandTokens).find(command) == (tools.mapCommandTokens).end())
        throw InvalidCommandToken(num, command);

    return (tools.mapCommandTokens)[command];
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
        int comment_pos = cell.find(';');
        if (comment_pos != -1)
            cell.erase(comment_pos);

        //  парсинг номера ячейки
        try{
            token = Tools::getToken(cell);
        }
        catch (EmptyFieldToken &)
        {
            continue;
        }

        // проверка на то, является ли прочитанная лексема числовой
        if (!Tools::number(token))
        {
            fin.close();
            throw InvalidAddressToken(cellNumber, token);
        }

        // все-таки числовая, проверим на выход за диапазон
        position = Tools::StrToInt(token, 10);

        if (position > 511 || position < 0)
        {
            fin.close();
            throw AddressOutRange(cellNumber, position, "Cell number out range!");
        }

        //   команда
        try {
            token = Tools::getToken(cell);
        }

        catch (EmptyFieldToken &)
        {
            fin.close();
            throw EmptyFieldToken(cellNumber, "Command");
        }

        // командам в памяти ум3 отводится 5 битов
        result += Tools::IntToStr(command_check(token, cellNumber), 2, 5);

        // op1, op2, op3
        for (int i = 0; i < 3; i++)
        {
            string opi = "Operand1"; opi[2] += i;

            try{
                token = Tools::getToken(cell);
            }

            catch (EmptyFieldToken &)
            {
                fin.close();
                throw EmptyFieldToken(cellNumber, opi);
            }

            // адреса операндов должны быть числами
            if (!Tools::number(token))
            {
                fin.close();
                throw InvalidAddressToken(cellNumber, token);
            }

            int token_val = Tools::StrToInt(token, 10);

            if (token_val > 511 || token_val < 0)
            {
                fin.close();
                throw AddressOutRange(cellNumber, token_val);
            }

            // каждому операнду отводится по 9 бит
            result += Tools::IntToStr(token_val, 2, 9);
        }

        mem_obj.push(position, result);
    }

    fin.close();
}