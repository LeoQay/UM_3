#pragma once

#include <string>

#include "Translator.h"
#include "Tools.h"
#include "parser.h"

using namespace std;

class Processor
{
private:
    Memory memory;
    Translator translator;

    string R1;           /* первый регистр АЛУ */
    string R2;           /* второй регистр АЛУ */
    int I1, I2;          // регистры АЛУ для целых
    long double F1, F2;  // регистры АЛУ для вещественных
    string Summator;     /* регистр сумматора (хранит результат) */
    int RA;              /* счетчик адреса, содержит адрес следующей команды  */
    bool Err;            /* регистр ошибок, true - произошла ошибка  */
    string RK;           /* текущая команда  */
    CommandCode RKcommand;
    int op1, op2, op3;   // current operands
    int omega;           /* 0 - результат равен 0, 1 - меньше 0, 2 - больше 0  */
    int iterations;
    int max_iterations;  /* не канонично, по умолчанию игнорируется */
    int saveRA;

    long long maxInt;
    long long minInt;
    long double maxFloat;
    long double minFloat;

    string punched_card_file_name;
    string log_file_name;

    void omega_res(int res);

    void omega_res(float res);

    void inInt();

    void outInt();

    void addInt();

    void subInt();

    void mulInt();

    void divInt();

    void modInt();

    void inFloat();

    void outFloat();

    void addFloat();

    void subFloat();

    void mulFloat();

    void divFloat();

    void intToFloat();

    void floatToInt();

    void unconditional();

    void PR ();

    void PNR ();

    void PB ();

    void PM ();

    void PBR ();

    void PMR ();

    void just_if();

    void mov();

    bool tact();

    void LoadRegisters (int& REG1, int& REG2, string command, string sign);

    void LoadRegisters (long double& REG1, long double& REG2, string command, string sign);

    void OutRangeChecker (long long res, CommandCode command);

    void OutRangeChecker (long double res, CommandCode command);

public:
    Processor();

    ~Processor();

    void set_PunchedCard(string punched_card_file_name);

    void Read_PunchedCard();

    void outMemory(string memory_file_name);

    void set_max_iterations(int num);

    void main_process();

    string output_stat();
};
