#pragma once

#include <string>

using namespace std;

/*
 *  Enum contain names of initial memory modes
 *  Random - all memory fulling randomly
 *  Zeros  - all cells in memory equal zero
 */
enum InitMemoryMode {
    Random,
    Zeros
};

enum FileFormat {
    txt,
    bin
};

class Config
{
public:
    InitMemoryMode initMemoryMode;

    FileFormat format_custom_memory;
    string custom_memory_file_name;


    FileFormat format_memory;
    string memory_file_name;


    FileFormat format_punched_card;
    string punched_card_file_name;


    const FileFormat format_log = FileFormat::txt;
    string log_file_name;


    const FileFormat format_start_machine_state = FileFormat::txt;
    string start_machine_state_file_name;



};
