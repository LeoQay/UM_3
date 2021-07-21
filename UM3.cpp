#pragma once

#include <iostream>

#include "Processor.h"
#include "Exception.h"

using namespace std;


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    char* first_arg = argv[1];

    Processor processor;

    try
    {
        processor.Load_PunchedCard();
    }

    catch (Exception & err)
    {
        cout << "Error in the line " << err.cell_number << "\n" << err.what() << "\n";

        return 1;
    }

    try{
        processor.main_process();
    }

    catch (Exception & err)
    {
        cout << "Error in the cell " << err.cell_number << "\n" << err.what() << "\n";

        return 1;
    }

    processor.outMemory();

    return 0;
}
