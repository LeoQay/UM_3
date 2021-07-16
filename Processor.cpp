#include <iostream>
#include <cmath>
#include <sstream>
#include <random>

#include "Processor.h"
#include "Tools.h"
#include "Translator.h"
#include "Parser.h"
#include "Exception.h"

void Processor::omega_res(int res)
{
    omega = res == 0 ? 0 : res < 0 ? 1 : 2;
}

void Processor::omega_res(float res)
{
    omega = res == 0 ? 0 : res < 0 ? 1 : 2;
}

Processor::Processor()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-2147483648, 2147483647);

    Register1 = dist(gen);
    Register2 = dist(gen);
    Summator = dist(gen);

    int buf1 = dist(gen), buf2 = dist(gen), buf3 = dist(gen);
    FRegister1 = Tools::IntToFloat(buf1);
    FRegister2 = Tools::IntToFloat(buf2);
    FSummator = Tools::IntToFloat(buf3);

    RA = 1;
    CurrentCommandAddress = 1;
    BreakPoint = -1;

    Err = false;

    omega = 0;

    RKcommand = CommandCode::END;
    op1 = 0, op2 = 0, op3 = 0;

/*
 * если не установить максимальное число итераций,
 * то iterations никогда не равен max_iterations
 */
    iterations = 0;
    max_iterations = -1;


    maxInt = 2147483647ll;
    minInt = -2147483648ll;

    punched_card_file_name = "punched_card.txt";
    memory_file_name       = "memory.txt";
    log_file_name          = "";
}

Processor::~Processor()
{

}

void Processor::set_PunchedCardFileName(string file_name)
{
    punched_card_file_name = file_name;
}

void Processor::set_MemoryFileName(string file_name)
{
    memory_file_name = file_name;
}

void Processor::set_LogFileName(string file_name)
{
    log_file_name = file_name;
}

void Processor::set_max_iterations(int num)
{
    max_iterations = num;
}

void Processor::set_BreakPoint(int NewBreakPoint)
{
    BreakPoint = NewBreakPoint;
}

void Processor::Load_PunchedCard()
{
    Translator::Translate(punched_card_file_name, memory);
}

void Processor::outMemory()
{
    memory.outNiceMemory(memory_file_name);
}

void Processor::inInt()
{
    int value;
    int counter = op2;
    int address = op1;

    while (counter-- > 0)
    {
        bool ok = false;

        cout << "Input int to " << "<"<<address<<">" << ":";

        while (!ok)
        {
            try {
                string token = Parser::getTokenInt();

                value = Tools::StrToInt(token, 10);

                ok = true;
            }

            catch (Exception &err) {
                cout << err.what() << "\nRewrite please:";
            }
        }

        memory.pushInt(address, value);
        address = (address + 1) % 512;
    }
}

void Processor::outInt()
{
    int counter = op2;
    int address = op1;

    while(counter-- > 0)
    {
        cout << "Int from " << "<" << address << ">" << ": " << memory.getInt(address) << "\n";
        address = (address + 1) % 512;
    }
}

void Processor::addInt()
{
    LoadIntRegisters();

    long long res = (long long)Register1 + (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);

    Summator = (int)res;
    memory.pushInt(op1, Summator);
}

void Processor::subInt()
{
    LoadIntRegisters();

    long long res = (long long)Register1 - (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);
    Summator = (int)res;

    memory.pushInt(op1, Summator);
}

void Processor::mulInt()
{
    LoadIntRegisters();

    long long res = (long long)Register1 * (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);
    Summator = (int)res;
    memory.pushInt(op1, Summator);
}

void Processor::divInt()
{
    LoadIntRegisters();

    if (Register2 == 0) throw NULL_DIVIDE(CurrentCommandAddress, (int)CommandCode::DIVINT, op1, op2, op3);

    long long res = (long long)Register1 / (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);
    Summator = (int)res;
    memory.pushInt(op1, Summator);
}

void Processor::modInt()
{
    LoadIntRegisters();

    if (Register2 == 0) throw NULL_DIVIDE(CurrentCommandAddress, (int)CommandCode::MOD, op1, op2, op3);

    long long res = (long long)Register1 % (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);
    Summator = (int)res;
    memory.pushInt(op1, Summator);
}

void Processor::inFloat()
{
    float value;

    int counter = op2;
    int address = op1;

    while (counter-- > 0)
    {
        bool ok = false;

        cout << "Input real to " << "<"<<address<<">" << ":";

        while (!ok)
        {
            try {
                string token = Parser::getTokenFloat();
                stringstream sss(token);
                sss >> value;
                ok = true;
            }

            catch (Exception &err) {
                cout << err.what() << "\nRewrite please:";
            }
        }

        memory.pushFloat(address, value);
        address = (address + 1) % 512;
    }
}

void Processor::outFloat()
{
    int counter = op2;
    int address = op1;

    while(counter-- > 0)
    {
        cout << "Real from " << "<"<<address<<">" << ": " << memory.getFloat(address) << "\n";

        address = (address + 1) % 512;
    }
}

void Processor::addFloat()
{
    LoadFloatRegisters();
    float res = FRegister1 + FRegister2;

    omega_res(res);

    FSummator = res;
    memory.pushFloat(op1, FSummator);
}

void Processor::subFloat()
{
    LoadFloatRegisters();
    float res = FRegister1 - FRegister2;

    omega_res(res);

    FSummator = res;

    memory.pushFloat(op1, FSummator);
}

void Processor::mulFloat()
{
    LoadFloatRegisters();
    float res = FRegister1 * FRegister2;

    omega_res(res);

    FSummator = res;
    memory.pushFloat(op1, FSummator);
}

void Processor::divFloat()
{
    LoadFloatRegisters();

    if (FRegister2 == 0) throw NULL_DIVIDE(CurrentCommandAddress, (int)CommandCode::DIVREAL, op1, op2, op3);

    float res = FRegister1 / FRegister2;

    omega_res(res);

    FSummator = res;
    memory.pushFloat(op1, FSummator);
}

void Processor::intToFloat ()
{
    float value = (float)memory.getInt(op3);
    memory.pushFloat(op1, value);
}

void Processor::floatToInt ()
{
    float F = memory.getFloat(op3);

    if (F < minInt || F > maxInt)
    {
        Err = true;
        throw FTOIOutRange(CurrentCommandAddress, (int)CommandCode::RTOI, op1, op2, op3, F);
    }

    memory.pushInt(op1, (int)F);
}

void Processor::unconditional ()
{
    RA = op2;
}

void Processor::PR()
{
    if (omega == 0)
    {
        RA = op2;
    }
}

void Processor::PNR ()
{
    if (omega != 0)
    {
        RA = op2;
    }
}

void Processor::PB ()
{
    if (omega == 2)
    {
        RA = op2;
    }
}

void Processor::PM ()
{
    if (omega == 1)
    {
        RA = op2;
    }
}

void Processor::PBR ()
{
    if (omega != 1)
    {
        RA = op2;
    }
}

void Processor::PMR ()
{
    if (omega != 2)
    {
        RA = op2;
    }
}

void Processor::conditional ()
{
    switch (omega)
    {
        case 0:
            RA = op1;
            break;
        case 1:
            RA = op2;
            break;
        case 2:
            RA = op3;
    }
}

void Processor::mov()
{
    memory.pushInt(op1, memory.getInt(op3));
}

bool Processor::tact()
{
    CurrentCommandAddress = RA;
    RA = (RA + 1) % 512;

    RK = memory.getStr(CurrentCommandAddress);
    Parser::cellParser(RK, RKcommand, op1, op2, op3);

    switch (RKcommand)
    {
        case CommandCode::ININT:
            inInt();
            break;
        case CommandCode::OUTINT:
            outInt();
            break;
        case CommandCode::ADDINT:
            addInt();
            break;
        case CommandCode::SUBINT:
            subInt();
            break;
        case CommandCode::MULINT:
            mulInt();
            break;
        case CommandCode::DIVINT:
            divInt();
            break;
        case CommandCode::MOD:
            modInt();
            break;
        case CommandCode::INREAL:
            inFloat();
            break;
        case CommandCode::OUTREAL:
            outFloat();
            break;
        case CommandCode::ADDREAL:
            addFloat();
            break;
        case CommandCode::SUBREAL:
            subFloat();
            break;
        case CommandCode::MULREAL:
            mulFloat();
            break;
        case CommandCode::DIVREAL:
            divFloat();
            break;
        case CommandCode::ITOR:
            intToFloat();
            break;
        case CommandCode::RTOI:
            floatToInt();
            break;
        case CommandCode::JUMP:
            unconditional();
            break;
        case CommandCode::PR:
            PR();
            break;
        case CommandCode::PNR:
            PNR();
            break;
        case CommandCode::PB:
            PB();
            break;
        case CommandCode::PM:
            PM();
            break;
        case CommandCode::PBR:
            PBR();
            break;
        case CommandCode::PMR:
            PMR();
            break;
        case CommandCode::IF:
            conditional();
            break;
        case CommandCode::MOV:
            mov();
            break;
        case CommandCode::END:
            return false;
        default:
            throw Bad_command(CurrentCommandAddress, (int)RKcommand, op1, op2, op3);
    }
    return true;
}

void Processor::main_process()
{
    do {
        BreakPointChecker();
    }while (!Err && iterations++ != max_iterations && tact());
}

void Processor::LoadIntRegisters()
{
    Register1 = memory.getInt(op2);
    Register2 = memory.getInt(op3);

}

void Processor::LoadFloatRegisters()
{
    FRegister1 = memory.getFloat(op2);
    FRegister2 = memory.getFloat(op3);
}

void Processor::OutRangeChecker(long long res)
{
    if (res < minInt || res > maxInt)
    {
        MathOutRange obj(CurrentCommandAddress, (int)RKcommand, op1, op2, op3, Register1, Register2);
        cout << "Warning!\n" << obj.what() << "\n";
    }
}

void Processor::BreakPointChecker()
{

}
