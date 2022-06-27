#ifndef UM_3_TRANSLATOR_H
#define UM_3_TRANSLATOR_H

#include <string>

#include "Memory.h"

class Translator
{
private:
    static int command_check (std::string command,
                              int         num
                              );

public:

/*
 *  Take text of program code from txt file and translate it to binary view
 *  Then put it into memory
 */
    static int Translate (std::string   PunchedCard_file_name,
                           Memory      & mem_obj
                           );

};

#endif //UM_3_TRANSLATOR_H
