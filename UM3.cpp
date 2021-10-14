#pragma once

#include <iostream>
#include <string>
#include <getopt.h>

#include "Convertor.h"
#include "Processor.h"
#include "Exception.h"
#include "Config.h"

using namespace std;

static struct option long_options[] = {
        {"config", 1, nullptr, 0},
        {"custom-mem", 1, nullptr, 0},
        {"start-state", 1, nullptr, 0},
        {"init-mem", 1, nullptr, 0},
        {"punch", 1, nullptr, 0},
        {"help", 0, nullptr, 0},
        {"run", 0, nullptr, 0},
        {"txt-bin", 1, nullptr, 0},
        {"bin-txt", 1, nullptr, 0},
        {"mem", 1, nullptr, 0},
        {"log", 1, nullptr, 0},
        {nullptr, 0, nullptr, 0}
};

enum index_of_options {
    CONFIG = 0,
    CUSTOM_MEMORY = 1,
    START_MACHINE_STATE = 2,
    INIT_MEMORY = 3,
    PUNCHED_CARD = 4,
    HELP = 5,
    START = 6,
    CONV_TXT = 7,
    CONV_BIN = 8,
    MEMORY = 9,
    LOG = 10
};

void command_line_reading(int argc, char* argv[], Config * config)
{
    string config_file_name;

    while(true)
    {
        int getopt_code;
        int option_index;

        getopt_code = getopt_long(argc, argv, "co:", long_options, &option_index);

        if(getopt_code == -1) break;

        switch(getopt_code)
        {
            case 'o':
                if(optarg) config->set_output_file_name(optarg);
                break;
            case 'c':
                config->set_exec_mode(ExecMode::ONLY_BIN);
                break;
            case 0:
            case 1:
            case 2:
                switch(option_index)
                {
                    case index_of_options::LOG:
                        config->set_log_file_name(optarg);
                        break;
                    case index_of_options::MEMORY:
                        if (optarg) config->set_memory_file_name(optarg);
                        break;
                    case index_of_options::CONV_TXT:
                        config->set_convert_file_name(optarg);
                        config->set_exec_mode(ExecMode::CONVERTOR_TXT_BIN);
                        break;
                    case index_of_options::CONV_BIN:
                        config->set_convert_file_name(optarg);
                        config->set_exec_mode(ExecMode::CONVERTOR_BIN_TXT);
                        break;
                    case index_of_options::START:
                        config->set_exec_mode(ExecMode::RUN);
                        break;
                    case index_of_options::INIT_MEMORY:
                        if(optarg) config->set_init_memory_mode(optarg);
                        break;
                    case index_of_options::CONFIG:
                        config_file_name = optarg;
                        break;
                    case index_of_options::CUSTOM_MEMORY:
                        if(optarg) config->set_custom_memory_file_name(optarg);
                        break;
                    case index_of_options::START_MACHINE_STATE:
                        if(optarg) config->set_start_machine_state_file_name(optarg);
                        break;
                    case index_of_options::PUNCHED_CARD:
                        if(optarg) config->set_punched_card_file_name(optarg);
                        break;
                    case index_of_options::HELP:
                        cout << config->help();
                        config->set_exec_mode(ExecMode::NOTHING);
                        return;
                        break;
                    default:
                        // ERROR
                        break;
                }
                break;
            default:
                throw Exception("Unknown option");
                break;
        }
    }

    config->set_config_file_name(config_file_name);
    config->loadConfigFile();
}


int main(int argc, char* argv[])
{
    try {
        Config * config = new Config;

        command_line_reading(argc, argv, config);

        if(config->get_exec_mode() == ExecMode::RUN) {
            Processor processor(config);

            processor.launch_button();

            if(config->get_memory_file_name() != "") {
                processor.outMemory();
            }
        } else if (config->get_exec_mode() == ExecMode::ONLY_BIN) {
            Convertor::punched_card_source_to_bin(
                    config->get_punched_card_file_name(),
                    config->get_output_file_name());
        } else if (config->get_exec_mode() == ExecMode::CONVERTOR_BIN_TXT) {
            Convertor::bin_to_txt(config->get_convert_file_name(),
                                  config->get_output_file_name());
        } else if (config->get_exec_mode() == ExecMode::CONVERTOR_TXT_BIN) {
            Convertor::txt_to_bin(config->get_convert_file_name(),
                                  config->get_output_file_name());
        }
    }


    catch (Exception & err)
    {
        cerr << "\n" << err.what() << "\n";
        return 1;
    }

    return 0;
}