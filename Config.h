#ifndef UM_3_CONFIG_H
#define UM_3_CONFIG_H

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

enum ExecMode {
    ONLY_BIN,
    RUN,
    CONVERTOR_TXT_BIN,
    CONVERTOR_BIN_TXT,
    NOTHING
};

class Config
{
private:
    map <string, ConfigFieldToken> mapConfigFieldToken;

    map <string, FileFormat> mapFileFormatToken;

    map <string, InitMemoryMode> mapInitMemoryToken;

    string convert_file_name;

    string output_file_name;

    string config_file_name;

    string custom_memory_file_name;

    string memory_file_name;

    string punched_card_file_name;

    string log_file_name;

    string start_machine_state_file_name;

    InitMemoryMode initMemoryMode;

    ExecMode executeMode;

    string help_msg = "It is simple emulator of UM-3\n"
                      "--txt-bin=<file name> - convert \"txt\" to binary\n"
                      "\n"
                      "--bin-txt=<file name> - convert binary to \"txt\"\n"
                      "\n"
                      "-o <file_name> - set output file name\n"
                      "\n"
                      "-c - program returns only binary file, required punched card\n"
                      "\n"
                      "--punched=<file_name> .txt or .bin - set executable file\n"
                      "\n"
                      "--init_memory=<RANDOM or ZEROS> - set initialization memory mode \n"
                      "\n"
                      "--config=<file_name.txt> - set config file\n"
                      "!!! options in config file more relevant than command line arguments !!!\n"
                      "\n"
                      "--start_machine_state=<file_name.txt> - set start machine state file\n"
                      "\n"
                      "--help - shows program arguments list\n";

public:
    Config();

    void set_exec_mode(ExecMode mode);
    ExecMode get_exec_mode();

    void   set_output_file_name(string file_name);
    string get_output_file_name();

    void           set_init_memory_mode(string mode_name);
    void           set_init_memory_mode(InitMemoryMode mode);
    InitMemoryMode get_init_memory_mode();


    void   set_convert_file_name(string file_name);
    string get_convert_file_name();


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

    string help();
};

#endif //UM_3_CONFIG_H
