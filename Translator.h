#pragma once

#include <string>

#include "Memory.h"
#include "Tools.h"

class Translator
{
private:
    int command_check (std::string command, int num);
    std::string getTokenCell (std::string& token, int num);

public:
    void Translate (std::string PunchedCard_file_name, Memory& mem_obj);


};

