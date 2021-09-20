#include <map>
#include <fstream>

#include "Config.h"
#include "Tools.h"
#include "Exception.h"

Config::Config()
{
    mapConfigFieldToken = {
            {"custom_memory_file_name",       ConfigFieldToken::CUSTOM_MEMORY_FILE_NAME        },
            {"memory_file_name",              ConfigFieldToken::MEMORY_FILE_NAME               },
            {"punched_card_file_name",        ConfigFieldToken::PUNCHED_CARD_FILE_NAME         },
            {"log_file_name",                 ConfigFieldToken::LOG_FILE_NAME                  },
            {"start_machine_state_file_name", ConfigFieldToken::START_MACHINE_STATE_FILE_NAME  },
            {"init_memory_mode",              ConfigFieldToken::INIT_MEMORY_MODE               }
    };

    mapFileFormatToken = {
            {"txt", FileFormat::TXT  },
            {"TXT", FileFormat::TXT  },

            {"bin", FileFormat::BIN  },
            {"BIN", FileFormat::BIN  },

            {"",    FileFormat::NONE }
    };

    mapInitMemoryToken = {
            {"RANDOM", InitMemoryMode::RANDOM },
            {"random", InitMemoryMode::RANDOM },

            {"ZEROS",  InitMemoryMode::ZEROS  },
            {"zeros",  InitMemoryMode::ZEROS  }
    };

    initMemoryMode                = InitMemoryMode::RANDOM;

    config_file_name              = "";

    punched_card_file_name        = "punched_card.txt";

    memory_file_name              = "";

    custom_memory_file_name       = "";

    start_machine_state_file_name = "";

    log_file_name                 = "";
}

void Config::loadConfigFile()
{
    ifstream config(config_file_name);

    if (!config.is_open())
        throw Exception("Config file -- " + config_file_name + " -- not found");

    while (!config.eof())
    {
        string line;
        string field_token;
        string name_token;

        getline(config, line);

        // remove comment
        int comment_pos = line.find('#');
        if (comment_pos != -1) line.erase(comment_pos);

        try {
            field_token = Tools::getToken(line);

            if (mapConfigFieldToken.find(field_token) == mapConfigFieldToken.end())
            {
                config.close();
                throw Exception(" Wrong configuration name -- " + field_token);
            }


            name_token  = Tools::getToken(line);
        }

        catch(EmptyFieldToken &) {
            continue;
        }


        switch (mapConfigFieldToken.find(field_token)->second)
        {
            case INIT_MEMORY_MODE:
                if (mapInitMemoryToken.find(name_token) == mapInitMemoryToken.end())
                    throw Exception("Error in config -- " + name_token);
                set_init_memory_mode(mapInitMemoryToken.find(name_token)->second);
            case CUSTOM_MEMORY_FILE_NAME:
                set_custom_memory_file_name(name_token);
                break;
            case MEMORY_FILE_NAME:
                set_memory_file_name(name_token);
                break;
            case PUNCHED_CARD_FILE_NAME:
                set_punched_card_file_name(name_token);
                break;
            case LOG_FILE_NAME:
                set_log_file_name(name_token);
                break;
            case START_MACHINE_STATE_FILE_NAME:
                set_start_machine_state_file_name(name_token);
                break;
            default:
                config.close();
                throw Exception("Error in config -- " + field_token + " " + name_token);
        }
    }

    config.close();
}


FileFormat Config::getFileFormat(string file_name)
{
    int dot = file_name.find('.');

    if (dot == -1) return FileFormat::NONE;

    file_name.erase(0, dot + 1);

    if (mapFileFormatToken.find(file_name) == mapFileFormatToken.end()) return FileFormat::NONE;

    return mapFileFormatToken.find(file_name)->second;
}


void Config::set_config_file_name(string file_name)
{
    config_file_name = file_name;
}

string Config::get_config_file_name()
{
    return config_file_name;
}


void Config::set_custom_memory_file_name(string file_name)
{
    custom_memory_file_name = file_name;
}

string Config::get_custom_memory_file_name()
{
    return custom_memory_file_name;
}


void Config::set_init_memory_mode(InitMemoryMode mode)
{
    initMemoryMode = mode;
}

InitMemoryMode Config::get_init_memory_mode()
{
    return initMemoryMode;
}


void Config::set_memory_file_name(string file_name)
{
    memory_file_name = file_name;
}

string Config::get_memory_file_name()
{
    return memory_file_name;
}


void Config::set_punched_card_file_name(string file_name)
{
    punched_card_file_name = file_name;
}

string Config::get_punched_card_file_name()
{
    return punched_card_file_name;
}

void Config::set_log_file_name(string file_name)
{
    log_file_name = file_name;
    ofstream fin(log_file_name);
    fin.clear();
    fin.close();
}

string Config::get_log_file_name()
{
    return log_file_name;
}


void Config::set_start_machine_state_file_name(string file_name)
{
    start_machine_state_file_name = file_name;
}

string Config::get_start_machine_state_file_name()
{
    return start_machine_state_file_name;
}


FileFormat Config::get_format_punched_card()
{
    return getFileFormat(punched_card_file_name);
}

