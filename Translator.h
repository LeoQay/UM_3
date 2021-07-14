#pragma once

#include <string>

#include "Memory.h"
#include "Tools.h"

class Translator
{
private:
    static int command_check (std::string command, int num);
    static std::string getTokenCell (std::string& token, int num);

public:
    static void Translate (std::string PunchedCard_file_name, Memory& mem_obj);

};

