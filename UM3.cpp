#pragma once

#include <iostream>
#include <fstream>

#include "Processor.h"
#include "Exception.h"

using namespace std;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    /*

    if(argc<4)
    {
        cout << "This program is an implementation of UM-3 computer\n"
                "We need to run it following file names in argumets:\n"
                "   punched_card.txt -- program code\n"
                "              (override default memory state)\n"
                "   log.txt -- file where all commands and operands\n"
                "              are stored\n\n";

        //return 1;
    }

    char *log_file_name=argv[3];
    char *punched_card_file_name=argv[2];
    char *memory_file_name=argv[1];

     */

    string default_punch = "punched_card.txt";
    string default_memory = "memory.txt";


    ifstream fin(default_punch);

    if (!fin.is_open())
    {
        cout << "File " << default_punch << " lost\n";
        return 1;
    }

    fin.close();

    Processor processor;

    try
    {
        processor.set_PunchedCard(default_punch);
        processor.Read_PunchedCard();
    }

    catch (Exception& err)
    {
        cout << "Error in the line " << err.cell_number << "\n" << err.what() << "\n";

        return 1;
    }

    try{
        processor.main_process();
    }

    catch (Exception& err)
    {
        cout << "Error in the cell " << err.cell_number << "\n" << err.what() << "\n";

        return 1;
    }

    processor.outMemory(default_memory);

    return 0;
}
