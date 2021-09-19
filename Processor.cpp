#include <iostream>
#include <sstream>
#include <fstream>

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

Processor::Processor(Config *config)
{
    this->config = config;

    memory.init_memory(config->get_init_memory_mode());

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

    print_log("INITIAL MACHINE STATE\n" + getMachineState() + "\n");
}


Processor::~Processor() = default;


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
    switch (config->get_format_punched_card()) {
        case FileFormat::TXT:
            load_punched_card_txt();
            break;
        case FileFormat::BIN:
            load_punched_card_bin();
            break;
        default:
            throw Exception("Punched card name unknown");
    }
}


void Processor::outMemory()
{
    memory.outNiceMemory(config->get_memory_file_name());
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

        memory.push(address, value);
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

    memory.push(op1, Summator);
}

void Processor::subInt()
{
    LoadIntRegisters();

    long long res = (long long)Register1 - (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);

    LoadSummator((int)res);

    memory.push(op1, Summator);
}

void Processor::mulInt()
{
    LoadIntRegisters();

    long long res = (long long)Register1 * (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);

    LoadSummator((int)res);

    memory.push(op1, Summator);
}

void Processor::divInt()
{
    LoadIntRegisters();

    if (Register2 == 0) throw NullDivide(CurrentCommandAddress, RK);

    long long res = (long long)Register1 / (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);

    LoadSummator((int)res);

    memory.push(op1, Summator);
}

void Processor::modInt()
{
    LoadIntRegisters();

    if (Register2 == 0) throw NullDivide(CurrentCommandAddress, RK);

    long long res = (long long)Register1 % (long long)Register2;

    OutRangeChecker(res);

    omega_res((int)res);

    LoadSummator((int)res);

    memory.push(op1, Summator);
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

        memory.push(address, value);
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

    memory.push(op1, FSummator);
}


void Processor::subFloat()
{
    LoadFloatRegisters();
    float res = FRegister1 - FRegister2;

    omega_res(res);

    LoadSummator(res);

    memory.push(op1, FSummator);
}


void Processor::mulFloat()
{
    LoadFloatRegisters();
    float res = FRegister1 * FRegister2;

    omega_res(res);

    LoadSummator(res);

    memory.push(op1, FSummator);
}


void Processor::divFloat()
{
    LoadFloatRegisters();

    if (FRegister2 == 0) throw NullDivide(CurrentCommandAddress, RK);

    float res = FRegister1 / FRegister2;

    omega_res(res);

    LoadSummator(res);

    memory.push(op1, FSummator);
}


void Processor::intToFloat ()
{
    float value = (float)memory.getInt(op3);
    memory.push(op1, value);
}


void Processor::floatToInt ()
{
    float F = memory.getFloat(op3);

    if (F < minInt || F > maxInt)
    {
        Err = true;
        throw FloatToIntOutRange(CurrentCommandAddress, RK, F);
    }

    memory.push(op1, (int) F);
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
    memory.push(op1, memory.getInt(op3));
}


bool Processor::tact()
{
    CurrentCommandAddress = RA;
    RA = (RA + 1) % 512;

    RK = memory.getStr(CurrentCommandAddress);
    Tools::ReadCell(RK, RKcommand, op1, op2, op3);

    print_log("\n" + Log::getNiceCommand(CurrentCommandAddress, RK) + "\n");

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
            throw UndefinedCommand(CurrentCommandAddress, (int)RKcommand, RK);
    }

    print_log("\nMACHINE STATE AFTER " + Tools::IntToStr(iterations, 10, 0) + " TACTS\n" + getMachineState());

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

    print_log(Log::BinaryOperator(RK, op2, op3, Register1, Register2));
}

void Processor::LoadFloatRegisters()
{
    FRegister1 = memory.getFloat(op2);
    FRegister2 = memory.getFloat(op3);

    Register1 = Tools::FloatToInt(FRegister1);
    Register2 = Tools::FloatToInt(FRegister2);

    print_log(Log::BinaryOperator(RK, op2, op3, Register1, Register2));
}

void Processor::LoadSummator(int value)
{
    Summator  = value;
    FSummator = Tools::IntToFloat(value);

    print_log(Log::getValueStr(Log::getAddressToken(op1), Summator));
}

void Processor::LoadSummator(float value)
{
    FSummator = value;
    Summator  = Tools::FloatToInt(value);

    print_log(Log::getValueStr(Log::getAddressToken(op1), Summator));
}

void Processor::OutRangeChecker(long long res)
{
    if (res < minInt || res > maxInt)
    {
        IntOutRange exception_opj(CurrentCommandAddress, RK, Register1, Register2, res);
        cout << "Warning!" << "\n" << exception_opj.what() << "\n";
    }
}


void Processor::BreakPointChecker()
{

}


string Processor::getMachineState()
{
    string token;
    token += Log::getTitle()                                                + "\n" +
             Log::getValueStr(      "R1", Register1)                  + "\n" +
             Log::getValueStr(      "R2", Register2)                  + "\n" +
             Log::getValueStr("Summator",  Summator)                  + "\n" +
             Log::getValueStr(      "RK", Tools::StrToInt(RK));
    return token;
}


void Processor::print_log(string message)
{
    if(config->get_log_file_name() == "") return;

    ofstream log(config->get_log_file_name(), ios::app);

    if(!log.is_open())
        throw Exception("Log file with name \"" + config->get_log_file_name() + "\" not found");

    log << message << "\n";

    log.close();
}


void Processor::set_config_file_name(string file_name)
{
    config->set_config_file_name(file_name);
    config->loadConfigFile();
}


void Processor::load_punched_card_txt()
{
    Translator::Translate(config->get_punched_card_file_name(), memory);
}


void Processor::load_punched_card_bin()
{
    memory.load_punched_card_bin(config->get_punched_card_file_name());
}
