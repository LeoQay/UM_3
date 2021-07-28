#include <map>
#include <fstream>

#include "Config.h"
#include "Tools.h"
#include "Exception.h"

Config::Config()
{
    mapConfigFieldToken = {
            {"custom_memory_file_name",       CUSTOM_MEMORY_FILE_NAME        },
            {"memory_file_name",              MEMORY_FILE_NAME               },
            {"punched_card_file_name",        PUNCHED_CARD_FILE_NAME         },
            {"log_file_name",                 LOG_FILE_NAME                  },
            {"start_machine_state_file_name", START_MACHINE_STATE_FILE_NAME  }
    };

    mapFileFormatToken = {
            {"txt", FileFormat::TXT  },
            {"TXT", FileFormat::TXT  },

            {"bin", FileFormat::BIN  },
            {"BIN", FileFormat::BIN  },

            {"",    FileFormat::NONE }
    };

    initMemoryMode                = InitMemoryMode::Random;

    config_file_name              = "config.txt";
    format_config_file            = FileFormat::TXT;

    punched_card_file_name        = "punched_card.txt";
    format_punched_card           = FileFormat::TXT;

    memory_file_name              = "memory.txt";
    format_memory                 = FileFormat::TXT;

    custom_memory_file_name       = "";
    format_custom_memory          = FileFormat::NONE;

    start_machine_state_file_name = "";
    format_start_machine_state    = FileFormat::NONE;

    log_file_name                 = "";
    format_log                    = FileFormat::NONE;
}

void Config::loadConfigFile()
{
    ifstream config(config_file_name);

    if (!config.is_open())
        throw Exception(-1, "Config file -- " + config_file_name + " -- not found");

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
            name_token  = Tools::getToken(line);
        }

        catch(Empty &) {
            continue;
        }

        switch (mapConfigFieldToken.find(field_token)->second)
        {
            case CUSTOM_MEMORY_FILE_NAME:
                break;
            case MEMORY_FILE_NAME:
                break;
            case PUNCHED_CARD_FILE_NAME:
                break;
            case LOG_FILE_NAME:
                break;
            case START_MACHINE_STATE_FILE_NAME:
                break;
            default:
                break;
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
    config_file_name   = file_name;
    format_config_file = getFileFormat(file_name);
}

string Config::get_config_file_name()
{
    return config_file_name;
}


void Config::set_custom_memory_file_name(string file_name)
{
    custom_memory_file_name = file_name;
    format_custom_memory    = getFileFormat(file_name);
}

string Config::get_custom_memory_file_name()
{
    return custom_memory_file_name;
}




