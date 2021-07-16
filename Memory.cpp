#pragma once

#include <fstream>
#include <random>

#include "Memory.h"
#include "Tools.h"

using namespace std;

Memory::Memory(InitMemory mode)
{
    if (mode == Random)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(-2147483648, 2147483647);

        for (int &var : memory_mas)
        {
            var = dist(gen);
        }
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


void Memory::pushStr(int index, std::string new_val)
{
    memory_mas[index] = Tools::StrToInt(move(new_val));
}

void Memory::pushInt(int index, int value)
{
    memory_mas[index] = value;
}

void Memory::pushFloat(int index, float value)
{
    memory_mas[index] = Tools::FloatToInt(value);
}

void Memory::outNiceMemory(string memory_file_name)
{
    ofstream answer(memory_file_name);

    for (int i = 0; i < 512; i++)
    {
        if (i < 10)
            answer << "00";
        else if (i < 100)
            answer << "0";

        answer << i << " " << getStr(i) << "\n";
    }
}
