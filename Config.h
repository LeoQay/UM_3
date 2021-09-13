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
    TXT = 0,
    BIN = 1,
    NONE = 2
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


    FileFormat format_config_file;
    string config_file_name;


    FileFormat format_custom_memory;
    string custom_memory_file_name;


    FileFormat format_memory;
    string memory_file_name;


    FileFormat format_punched_card;
    string punched_card_file_name;


    FileFormat format_log;
    string log_file_name;


    FileFormat format_start_machine_state;
    string start_machine_state_file_name;


    InitMemoryMode initMemoryMode;


    FileFormat getFileFormat(string file_name);

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


    void   set_log_file_name(string file_name);
    string get_log_file_name();


    void   set_start_machine_state_file_name(string file_name);
    string get_start_machine_state_file_name();


    void loadConfigFile ();

};
