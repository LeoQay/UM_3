#pragma once

#include <fstream>

#include "Memory.h"
#include "Tools.h"
#include "Exception.h"

using namespace std;

Memory::Memory()
{

}


void Memory::random_init_memory()
{
    for (int & var : memory_mas)
    {
        var = Tools::getRandomInt();
    }
}


void Memory::zeros_init_memory()
{
    for (int & var : memory_mas)
    {
        var = 0;
    }
}


void Memory::init_memory(InitMemoryMode mode)
{
    switch (mode)
    {
        case InitMemoryMode::RANDOM:
            random_init_memory();
            break;
        case InitMemoryMode::ZEROS:
            zeros_init_memory();
            break;
        default:
            throw Exception("Invalid init memory mode");
    }
}


std::string Memory::getStr(int index)
{
    return Tools::IntToStr(memory_mas[index]);
}


int Memory::getInt(int index)
{
    return memory_mas[index];
}


float Memory::getFloat(int index)
{
    return Tools::IntToFloat(memory_mas[index]);
}


void Memory::push(int index, std::string new_val)
{
    memory_mas[index] = Tools::StrToInt(move(new_val));
}


void Memory::push(int index, int value)
{
    memory_mas[index] = value;
}


void Memory::push(int index, float value)
{
    memory_mas[index] = Tools::FloatToInt(value);
}


void Memory::outNiceMemory(string memory_file_name)
{
    ofstream answer(memory_file_name);

    for (int i = 0; i < 512; i++)
    {
        answer << Tools::getRightBorderStr(Tools::IntToStr(i, 10, 0), 3, '0')
               << " "
               << getStr(i)
               << "\n";
    }

    answer.close();
}


void Memory::load_punched_card_bin(string punched_card_file_name)
{
    fstream punched_card(punched_card_file_name, ios::binary|ios::out);

    if(!punched_card.is_open())
        throw Exception("Punched card file not found");

    int buffer;
    int current_value = memory_mas[0];
    int index = 0;

    while(punched_card >> buffer)
    {
        memory_mas[index] = current_value;

        index = (index + 1) % 512;

        current_value = buffer;
    }

    punched_card.close();
}
