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

        for (int &var : mem_mas)
        {
            var = dist(gen);
        }
    }
}

std::string Memory::get(int index)
{
    return IntToStr(mem_mas[index]);
}

void Memory::push(int index, std::string new_val)
{
    mem_mas[index] = StrToInt(move(new_val));
}

void Memory::outMemory(string memory_file_name)
{
    ofstream answer(memory_file_name);

    for (int i = 0; i < 512; i++)
    {
        if (i < 10)
            answer << "  ";
        else if (i < 100)
            answer << " ";

        answer << i << " " << get(i) << "\n";
    }
}

int Memory::StrToInt(std::string cell)
{
    int value = 0;

    for (int i = 0; i < 32; i++)
        if (cell[i] == '1')
            value = 2 * value + 1;
        else
            value = 2 * value;

    return value;
}

std::string Memory::IntToStr(int value)
{
    std::string cell = "";

    for (int i = 0; i < 32; i++)
    {
        if (value & 1)
            cell = "1" + cell;
        else
            cell = "0" + cell;

        value = value >> 1;
    }

    return cell;
}