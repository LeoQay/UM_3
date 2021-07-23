#include <iostream>
#include <sstream>

#include "Processor.h"
#include "Tools.h"
#include "Translator.h"
#include "Parser.h"
#include "Exception.h"
#include "Log.h"
#include "Config.h"

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
    Register1 = Tools::getRandomInt();
    Register2 = Tools::getRandomInt();
    Summator  = Tools::getRandomInt();

    FRegister1 = Tools::IntToFloat(Register1);
    FRegister2 = Tools::IntToFloat(Register2);
    FSummator  = Tools::IntToFloat(Summator);

    RK = Tools::IntToStr(Tools::getRandomInt());
    Tools::ReadCell(RK, RKcommand, op1, op2, op3);

    RA = 1;
    CurrentCommandAddress = 1;
    BreakPoint = -1;

    Err = false;

    omega = 0;

/*
 * если не установить максимальное число итераций,
 * то iterations никогда не равен max_iterations
 */
    iterations = 0;
    max_iterations = -1;


    maxInt = 2147483647ll;
    minInt = -2147483648ll;

    config.punched_card_file_name = "punched_card.txt";
    config.memory_file_name       = "memory.txt";
    config.log_file_name          = "";

    cout << "INITIAL MACHINE STATE" << "\n"
         << getMachineState() << "\n";
}

Processor::~Processor()
{

}

void Processor::set_PunchedCardFileName(string file_name)
{
    config.punched_card_file_name = std::move(file_name);
}

void Processor::set_MemoryFileName(string file_name)
{
    config.memory_file_name = std::move(file_name);
}

void Processor::set_LogFileName(string file_name)
{
    config.log_file_name = std::move(file_name);
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
    Translator::Translate(config.punched_card_file_name, memory);
}

void Processor::outMemory()
{
    memory.outNiceMemory(config.memory_file_name);
}

void Processor::inInt()
{
    int value;
    int counter = op2;
    int address = op1;

    while (counter-- > 0)
    {
        bool ok = false;

        while (!ok)
        {
            try {
                cout << "Input int to " << "<"<<address<<">" << ":";

                string token = Parser::getTokenInt();

                value = Tools::StrToInt(token, 10);

                ok = true;
            }

            catch (Exception & err) {
                cout << err.what() << "\n";
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

    LoadSummator((int)res);

    memory.pushInt(op1, Summator);
}

void Processor::subInt()
{
    LoadIntRegisters();

    long long res = (long long)Register1 - (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);

    LoadSummator((int)res);

    memory.pushInt(op1, Summator);
}

void Processor::mulInt()
{
    LoadIntRegisters();

    long long res = (long long)Register1 * (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);

    LoadSummator((int)res);

    memory.pushInt(op1, Summator);
}

void Processor::divInt()
{
    LoadIntRegisters();

    if (Register2 == 0) throw NULL_DIVIDE(CurrentCommandAddress, (int)CommandCode::DIVINT, op1, op2, op3);

    long long res = (long long)Register1 / (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);

    LoadSummator((int)res);

    memory.pushInt(op1, Summator);
}

void Processor::modInt()
{
    LoadIntRegisters();

    if (Register2 == 0) throw NULL_DIVIDE(CurrentCommandAddress, (int)CommandCode::MOD, op1, op2, op3);

    long long res = (long long)Register1 % (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);

    LoadSummator((int)res);

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

        while (!ok)
        {
            try {
                cout << "Input real to " << "<"<<address<<">" << ":";

                string token = Parser::getTokenFloat();
                stringstream sss(token);
                sss >> value;
                ok = true;
            }

            catch (Exception & err) {
                cout << err.what() << "\n";
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

    LoadSummator(res);

    memory.pushFloat(op1, FSummator);
}

void Processor::subFloat()
{
    LoadFloatRegisters();
    float res = FRegister1 - FRegister2;

    omega_res(res);

    LoadSummator(res);

    memory.pushFloat(op1, FSummator);
}

void Processor::mulFloat()
{
    LoadFloatRegisters();
    float res = FRegister1 * FRegister2;

    omega_res(res);

    LoadSummator(res);

    memory.pushFloat(op1, FSummator);
}

void Processor::divFloat()
{
    LoadFloatRegisters();

    if (FRegister2 == 0) throw NULL_DIVIDE(CurrentCommandAddress, (int)RKcommand, op1, op2, op3);

    float res = FRegister1 / FRegister2;

    omega_res(res);

    LoadSummator(res);

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
        throw FTOIOutRange(CurrentCommandAddress, (int)RKcommand, op1, op2, op3, F);
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
    Tools::ReadCell(RK, RKcommand, op1, op2, op3);

    cout << "\n" << Log::getNiceCommand(CurrentCommandAddress, RK) << "\n\n";

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

    cout << "\n" << "MACHINE STATE AFTER " << iterations << " TACTS" << "\n"
         << getMachineState() << "\n";

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

    FRegister1 = Tools::IntToFloat(Register1);
    FRegister2 = Tools::IntToFloat(Register2);

    cout << Log::BinaryOperator(RK, op2, op3, Register1, Register2) << "\n";
}

void Processor::LoadFloatRegisters()
{
    FRegister1 = memory.getFloat(op2);
    FRegister2 = memory.getFloat(op3);

    Register1 = Tools::FloatToInt(FRegister1);
    Register2 = Tools::FloatToInt(FRegister2);

    cout << Log::BinaryOperator(RK, op2, op3, Register1, Register2) << "\n";
}

void Processor::LoadSummator(int value)
{
    Summator  = value;
    FSummator = Tools::IntToFloat(value);

    cout << Log::getValueStr(Log::getAddressToken(op1), Summator) << "\n";
}

void Processor::LoadSummator(float value)
{
    FSummator = value;
    Summator  = Tools::FloatToInt(value);

    cout << Log::getValueStr(Log::getAddressToken(op1), Summator) << "\n";
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

string Processor::getMachineState()
{
    string token;
    token += Log::getTitle() + "\n" +
             Log::getValueStr("R1", Register1) + "\n" +
             Log::getValueStr("R2", Register2) + "\n" +
             Log::getValueStr("Summator", Summator) + "\n" +
             Log::getValueStr("RK", Tools::StrToInt(RK));
    return token;
}
