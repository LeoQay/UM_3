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
        {"config", 2, nullptr, 0},
        {"custom_memory", 2, nullptr, 0},
        {"start_machine_state", 2, nullptr, 0},
        {"init_memory", 2, nullptr, 0},
        {"punched", 1, nullptr, 0},
        {"help", 0, nullptr, 0},
        {nullptr, 0, nullptr, 0}
};

enum index_of_options {
    CONFIG = 0,
    CUSTOM_MEMORY = 1,
    START_MACHINE_STATE = 2,
    INIT_MEMORY = 3,
    PUNCHED_CARD = 4,
    HELP = 5
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
                // ERROR
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

        Processor processor(config);

        if(config->get_exec_mode() == ExecMode::RUN) {
            processor.launch_button();

            if(config->get_memory_file_name() != "") {
                processor.outMemory();
            }
        } else if (config->get_exec_mode() == ExecMode::ONLY_BIN) {
            Convertor::punched_card_source_to_bin(
                    config->get_punched_card_file_name(),
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