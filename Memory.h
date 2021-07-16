#pragma once

/*
 *  Enum contain names of initial memory modes
 *  Random - all memory fulling randomly
 *  Zeros  - all cells in memory equal zero
 *  Custom - memory loading from user file
 */
enum InitMemory {
    Random,
    Zeros,
    Custom
};


/*
 *  Class interacts with memory
 *
 */
class Memory
{
private:
    int memory_size = 512;

/*
 *  Basically, it is memory which contain all cells
 */
    int memory_mas[512];
public:
    Memory(InitMemory mode = Random);

/*
 *  Each cell can be perceived as integer or as float,
 *  so various methods of interaction are implemented
 */

/*
 *  getStr and pushStr perceive cell as string of char ('0', '1')
 *  to show memory inside
 */

    int getInt (int index);
    float getFloat (int index);
    std::string getStr(int index);

    void pushStr(int index, std::string new_val);
    void pushInt(int index, int value);
    void pushFloat(int index, float value);

/*
 *
 */
    void outNiceMemory(std::string memory_file_name);
};
