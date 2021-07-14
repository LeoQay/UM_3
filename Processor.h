#pragma once

#include <string>

#include "Translator.h"
#include "Parser.h"

using namespace std;

class Processor
{
private:
/*
 *  memory - this is an object for storing memory
 */
    Memory memory;

/*
 *  It's ALU (arithmetic logic unit) registers
 *  They're used for calculating
 *  The registers are duplicated for various operations with integers and real values
 *  All calculations are performed as follows:
 *  Loading values from memory to R1 and R2
 *  Summator = R1 (operation) R2
 *  Loading result from Summator to memory
 */

    int Register1;
    int Register2;
    int Summator;

    float FRegister1;
    float FRegister2;
    float FSummator;

/*
 *  RA - Register Address
 *  It's contains address of next command
 */
    int RA;

/*
 *  It's contains address of next command which must be interrupted
 */
    int BreakPoint;

/*
 *  address of current command
 */
    int CurrentCommandAddress;

/*
 *  This fields are contain current command as string
 *  and in a parsed form
 *  command op1 op2 op3
 */
    string RK;
    CommandCode RKcommand;
    int op1, op2, op3;

/*
 *  This register signals an error
 */
    bool Err;

/*
 *  The register takes a value depending on the result of the last calculation
 *  if result == 0 --> omega = 0
 *  if result < 0  --> omega = 1
 *  if result > 0  --> omega = 2
 *  This is used for conditional operators
 */
    int omega;

/*
 *
 */
    int iterations;
    int max_iterations;  /* не канонично, по умолчанию игнорируется */

    long long maxInt;
    long long minInt;


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


    void LoadIntRegisters ();
    void LoadFloatRegisters ();

    void OutRangeChecker (long long res);

    void BreakPointChecker ();

    bool tact();

public:
    Processor();

    ~Processor();

    void set_PunchedCard(string punched_card_file_name);

    void Read_PunchedCard();

    void outMemory(string memory_file_name);

    void set_max_iterations(int num);

    void set_BreakPoint (int NewBreakPoint);

    void main_process();
};
