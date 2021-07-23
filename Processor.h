#pragma once

#include <string>

#include "Memory.h"
#include "Tools.h"
#include "Config.h"

using namespace std;

class Processor
{
private:
/*
 *  memory - this is an object for storing memory
 */
    Memory memory;

    Config config;

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
 *  and in a parsed form (command -- op1 -- op2 -- op3)
 */
    string RK;
    CommandCode RKcommand;
    int op1, op2, op3;

/*
 *  This register signals an error
 *  Err = true if error was happened
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
 *  This variables used for limitation of computations
 *  "iterations" contain number of completed tacts
 *  "max_iterations" contain max number of completed tacts
 *  By default it isn't used until "max_iterations" is settled
 */
    int iterations;
    int max_iterations;

/*
 *  used to check integer out range
 */
    long long maxInt;
    long long minInt;

/*
 *  functions for updating omega after new result
 */
    void omega_res(int res);
    void omega_res(float res);

/*
 *  Every cell may be perceived as data or as command
 *  If the UM-3 machine treats the cell as a command
 *  the following information is extracted from the cell
 *  Command Code - it's determine which command be executed
 *  op1          - they're three integer numbers from range (0 - 511)
 *  op2          - which are used by different command in different ways
 *  op3          - as arguments
 */

/*
 *  inInt - input integer
 *  CODE = 16 = "10000"
 *  This command accepts integers from the console
 *  The numbers are entered to memory sequentially, starting from the address op1
 *  Total numbers are entered op2
 *  op3 not used and can be equal to anything without changing the result
 */
    void inInt();

/*
 *  outInt - output integer
 *  CODE = 17 = "10001"
 *  This command output integers to the console
 *  The numbers are output from memory sequentially, starting from the address op1
 *  Total numbers are entered op2
 *  op3 not used and can be equal to anything without changing the result
 */
    void outInt();

/*
 *  addInt - add integer
 *  CODE = 11 = "01011"
 *  This command adds integers from the addresses op2 and op3
 *  and puts the result at the address op1
 */
    void addInt();

/*
 *  subInt - subtract integer
 *  CODE = 12 = "01100"
 *  This command subtracts integers from the addresses op2 and op3
 *  and puts the result at the address op1
 */
    void subInt();

/*
 *  mulInt - multiply integer
 *  CODE = 13 = "01101"
 *  This command multiplies integers from the addresses op2 and op3
 *  and puts the result at the address op1
 */
    void mulInt();

/*
 *  divInt - divide integer
 *  CODE = 14 = "01110"
 *  This command divides integers from the addresses op2 and op3
 *  and puts the result at the address op1
 */
    void divInt();

/*
 *  modInt - module integer
 *  CODE = 15 = "01111"
 *  This command divides by modulo integers from the addresses op2 and op3
 *  and puts the result at the address op1
 */
    void modInt();

/*
 *  inFloat - input float
 *  CODE = 6 = "00110"
 *  This command accepts float numbers from the console
 *  The numbers are entered to memory sequentially, starting from the address op1
 *  Total numbers are entered op2
 *  op3 not used and can be equal to anything without changing the result
 */
    void inFloat();

/*
 *  outFloat - output float
 *  CODE = 7 = "00111"
 *  This command output float numbers to the console
 *  The numbers are output from memory sequentially, starting from the address op1
 *  Total numbers are entered op2
 *  op3 not used and can be equal to anything without changing the result
 */
    void outFloat();

/*
 *  addFloat - add float numbers
 *  CODE = 1 = "00001"
 *  This command adds float numbers from the addresses op2 and op3
 *  and puts the result at the address op1
 */
    void addFloat();

/*
 *  subFloat - subtract float numbers
 *  CODE = 2 = "00010"
 *  This command subtracts float numbers from the addresses op2 and op3
 *  and puts the result at the address op1
 */
    void subFloat();

/*
 *  mulFloat - multiply float numbers
 *  CODE = 3 = "00011"
 *  This command multiplies float numbers from the addresses op2 and op3
 *  and puts the result at the address op1
 */
    void mulFloat();

/*
 *  divInt - divide float numbers
 *  CODE = 4 = "00100"
 *  This command divides float numbers from the addresses op2 and op3
 *  and puts the result at the address op1
 */
    void divFloat();

/*
 *  Convert integer to float number
 *  CODE = 30 = "11110"
 *  This command converts an integer at the address op3 to a float number
 *  and puts it at the address op1
 */
    void intToFloat();

/*
 *  Convert float number to integer
 *  CODE = 10 = "01010"
 *  This command converts an float number at the address op3 to an integer
 *  and puts it at the address op1
 */
    void floatToInt();



/*
 *  The following commands are conditional jump commands
 *  They make a jump to one of the addresses op1, op2, op3,
 *  depending on the value of the omega register,
 *  The next command is taken at this address
 */


/*
 *  CODE = 9 = "01001"
 *  This command doesn't check omega register
 *  and make a jump to address op2
 */
    void unconditional();

/*
 *  "Perehod Ravno"
 *  CODE = 20 = "10100"
 *  Like "je" in assembler
 *  Make a jump to address op2 if omega == 0
 *  Else it goes to the next command in memory
 */
    void PR ();

/*
 *  "Perehod Ne Ravno"
 *  CODE = 21 = "10101"
 *  Like "jne" in assembler
 *  Make a jump to address op2 if omega != 0
 *  Else it goes to the next command in memory
 */
    void PNR ();

/*
 *  "Perehod Bolshe"
 *  CODE = 22 = "10110"
 *  Like "jg" in assembler
 *  Make a jump to address op2 if omega == 2
 *  Else it goes to the next command in memory
 */
    void PB ();

/*
 *  "Perehod Menshe"
 *  CODE = 23 = "10111"
 *  Like "jl" in assembler
 *  Make a jump to address op2 if omega == 1
 *  Else it goes to the next command in memory
 */
    void PM ();

/*
 *  "Perehod Bolshe Ravno"
 *  CODE = 24 = "11000"
 *  Like "jge" in assembler
 *  Make a jump to address op2 if (omega == 2 OR omega == 0)
 *                 which equal if (omega != 1)
 *  Else it goes to the next command in memory
 */
    void PBR ();

/*
 *  "Perehod Menshe Ravno"
 *  CODE = 25 = "11001"
 *  Like "jle" in assembler
 *  Make a jump to address op2 if (omega == 1 OR omega == 0)
 *                 which equal if (omega != 2)
 *  Else it goes to the next command in memory
 */
    void PMR ();

/*
 *  CODE = 19 = "10011"
 *  Depending on the value of the omega register
 *  Command make jump to address
 *  op1   if   omega = 0
 *  op2   if   omega = 1
 *  op3   if   omega = 2
 */
    void conditional();

/*
 *  CODE = 0 = "00000"
 *  Like "mov" in assembler
 *  Takes the value at the address op3 and puts it at the address op1
 */
    void mov();

/*
 *  Auxiliary functions that are used to load numbers from memory into the ALU registers
 */
    void LoadIntRegisters   ();
    void LoadFloatRegisters ();

    void LoadSummator (int   value);
    void LoadSummator (float value);

/*
 *  This function used for warning about overflow in integer calculations
 */
    void OutRangeChecker (long long res);

/*
 *  It's function wasn't finished
 */
    void BreakPointChecker ();

/*
 *  This function make one tact of UM3 machine
 *  Execute one command and choose next command
 */
    bool tact();

public:
    Processor();

    ~Processor();

/*
 *  This function translate program code from punched_card_file (txt)
 *  to binary view and load it to memory object
 */
    void Load_PunchedCard();

/*
 *  This function create or rewrite memory_file (txt)
 *  and output current memory state into it
 */
    void outMemory();

/*
 *  Basically, this function is launch button
 *  it execute tact one by one before end command or error or breakpoint
 */
    void main_process();

    void set_PunchedCardFileName (string file_name);
    void set_MemoryFileName (string file_name);
    void set_LogFileName (string file_name);
    void set_max_iterations(int num);
    void set_BreakPoint (int NewBreakPoint);

    string getMachineState ();
};
