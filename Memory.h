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
public:
    Memory(InitMemory mode = Random);

    int getInt (int index);
    float getFloat (int index);
    std::string getStr(int index);

    void pushStr(int index, std::string new_val);
    void pushInt(int index, int value);
    void pushFloat(int index, float value);

    void outNiceMemory(std::string memory_file_name);
};
