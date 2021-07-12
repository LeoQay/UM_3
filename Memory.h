#pragma once

enum InitMemory{
    Random,
    Zeros,
    Custom
};

class Memory
{
private:
    int mem_size = 512;
    int mem_mas[512];

    int StrToInt (std::string cell);

    std::string IntToStr (int value);
public:
    Memory(InitMemory mode = Random);

    std::string get(int index);

    void push(int index, std::string new_val);

    void outMemory(std::string memory_file_name);
};
