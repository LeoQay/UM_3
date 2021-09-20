#pragma once

#include <string>
#include <map>

using namespace std;

/*
 *  Enum contain names of initial memory modes
 *  Random - all memory fulling randomly
 *  Zeros  - all cells in memory equal zero
 */
enum InitMemoryMode {
    RANDOM,
    ZEROS
};

enum FileFormat {
    TXT = 1,
    BIN = 2,
    NONE = 0
};

enum ConfigFieldToken {
    CUSTOM_MEMORY_FILE_NAME,
    MEMORY_FILE_NAME,
    PUNCHED_CARD_FILE_NAME,
    LOG_FILE_NAME,
    START_MACHINE_STATE_FILE_NAME,
    INIT_MEMORY_MODE
};

class Config
{
private:
    map <string, ConfigFieldToken> mapConfigFieldToken;

    map <string, FileFormat> mapFileFormatToken;

    map <string, InitMemoryMode> mapInitMemoryToken;

    string config_file_name;

    string custom_memory_file_name;

    string memory_file_name;

    string punched_card_file_name;

    string log_file_name;

    string start_machine_state_file_name;

    InitMemoryMode initMemoryMode;

public:
    Config();


    void           set_init_memory_mode(InitMemoryMode mode);
    InitMemoryMode get_init_memory_mode();


    void   set_config_file_name(string file_name);
    string get_config_file_name();


    void   set_custom_memory_file_name(string file_name);
    string get_custom_memory_file_name();


    void   set_memory_file_name(string file_name);
    string get_memory_file_name();


    void   set_punched_card_file_name(string file_name);
    string get_punched_card_file_name();


    FileFormat get_format_punched_card();

    FileFormat getFileFormat(string file_name);


    void   set_log_file_name(string file_name);
    string get_log_file_name();


    void   set_start_machine_state_file_name(string file_name);
    string get_start_machine_state_file_name();


    void loadConfigFile ();

};
