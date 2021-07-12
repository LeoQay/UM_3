#pragma once

#include <iostream>

#include "Processor.h"
#include "Exception.h"
#include "Tools.h"

using namespace std;

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    cout << Tools::stoi("111111111111111111111111");

    if(argc<4)
    {
        cout << "This program is an implementation of UM-3 computer\n"
                "We need to run it following file names in argumets:\n"
                "   memory.txt -- final state of memory\n"
                "   punched_card.txt -- program code\n"
                "              (override default memory state)\n"
                "   log.txt -- file where all commands and operands\n"
                "              are stored\n\n";

        //return 1;
    }

    char *log_file_name=argv[3];
    char *punched_card_file_name=argv[2];
    char *memory_file_name=argv[1];

    string default_punch = "punched_card.txt";
    string default_memory = "memory.txt";

    /*
    ifstream fin("punched_card.txt");

    if (!fin.is_open())
    {
        cout << "File 'punched_card.txt' lost\n";
        return 1;
    }

    ofstream fout("memory.txt");
    ofstream logFile("log.txt");
    */

    Processor processor;

    try
    {
        processor.set_PunchedCard(default_punch);
        processor.Read_PunchedCard();
    }

    catch (Exception& err)
    {
        // logFile << "Punched card load fail\n";
        cout << "\nError in the line " << err.cell_number << "\n" << err.what() << "\n\n";
        // logFile << "\nError in the line " << err.cell_number << "\n" << err.what() << "\n\n";

        return 1;
    }

    try{
        processor.main_process();
    }

    catch (Exception& err)
    {
        cout << "\nError in the cell " << err.cell_number << "\n" << err.what() << "\n\n";
        // logFile << "\nError in the cell " << err.cell_number << "\n" << err.what() << "\n\n";

        return 1;
    }

    processor.outMemory(default_memory);

    return 0;
}
