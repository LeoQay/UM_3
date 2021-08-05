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
        case InitMemoryMode::Random:
            random_init_memory();
            break;
        case InitMemoryMode::Zeros:
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
