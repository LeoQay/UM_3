#pragma once

#include <iostream>
#include <getopt.h>

#include "Convertor.h"
#include "Processor.h"
#include "Exception.h"

using namespace std;

static struct option long_options[] = {
        {"config",              2, nullptr, 0},
        {"custom_memory",       2, nullptr, 0},
        {"start_machine_state", 2, nullptr, 0},
        {"punched_card",        2, nullptr, 0},
        {nullptr,               0, nullptr, 0}
};

enum index_of_options {
    CONFIG = 0,
    CUSTOM_MEMORY = 1,
    START_MACHINE_STATE = 2,
    PUNCHED_CARD = 3
};

void command_line_reading(int argc, char* argv[], Config * config)
{
    Config buffer_config;

    while(true)
    {
        int getopt_code;
        int option_index;

        getopt_code = getopt_long(argc, argv, "", long_options, &option_index);

        if(getopt_code == -1) break;

        switch(getopt_code)
        {
            case 0:
                switch(option_index)
                {
                    case index_of_options::CONFIG:
                        if(optarg) config->set_config_file_name(optarg);
                        config->loadConfigFile();
                        cout << "CONFIG " << optarg << "\n";
                        break;
                    case index_of_options::CUSTOM_MEMORY:
                        if(optarg) buffer_config.set_custom_memory_file_name(optarg);
                        cout << "CUSTOM " << optarg << "\n";
                        break;
                    case index_of_options::START_MACHINE_STATE:
                        if(optarg) buffer_config.set_start_machine_state_file_name(optarg);
                        cout << "START MACHINE STATE " << optarg << "\n";
                        break;
                    case index_of_options::PUNCHED_CARD:
                        if(optarg) buffer_config.set_punched_card_file_name(optarg);
                        cout << "PUNCHED CARD " << optarg << "\n";
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


}


int main(int argc, char* argv[])
{
    try {
        Config * config = new Config;

        command_line_reading(argc, argv, config);


        Processor processor(config);

        processor.Load_PunchedCard();

        processor.main_process();

        processor.outMemory();
    }
    catch (Exception & err)
    {
        cerr << "\n" << err.what() << "\n";
        return 1;
    }

    return 0;
}
