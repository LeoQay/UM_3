#pragma once

#include <string>

#include "Memory.h"

class Translator
{
private:
    static int command_check (std::string command,
                              int         num
                              );

    static std::string getTokenCell (std::string & token,
                                     int           num
                                     );

public:

/*
 *  Take text of program code from txt file and translate it to binary view
 *  Then put it into memory
 */
    static void Translate (std::string   PunchedCard_file_name,
                           Memory      & mem_obj
                           );

};

