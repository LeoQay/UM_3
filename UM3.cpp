#pragma once

#include <iostream>

#include "Processor.h"
#include "Exception.h"

using namespace std;


int main(int argc, char* argv[])
{
    Config config;

    try {
        config.loadConfigFile();
    }

    catch (Exception & err)
    {
        cout << "\n" << err.what() << "\n";
    }

    Processor processor;

    try
    {
        processor.Load_PunchedCard();
    }

    catch (Exception & err)
    {
        cout << "\n" << err.what() << "\n";
        return 1;
    }

    try{
        processor.main_process();
    }

    catch (Exception & err)
    {
        cout << "\n" << err.what() << "\n";
        return 1;
    }

    processor.outMemory();

    return 0;
}
