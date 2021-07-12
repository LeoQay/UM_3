#pragma once

#include <map>

#include "Memory.h"

#include "Tools.h"

class Parser
{
public:
    static void cellParser (std::string& s, CommandCode& command, int& op1, int& op2, int& op3);

    static std::string getTokenInt();
    static float getTokenFloat();
};
