#pragma once

#include <string>

using namespace std;

class Log
{
private:
    string title_array [5] = {
            "NAME",
            "INT",
            "REAL",
            "STRING",
            "COMMAND"
    };

    int length_title_array [5] = {
            10,
            12,
            15,
            34,
            20
    };

    string getName (string name);

    string getInt (int value);

    string getReal (int value);

    string getString (int value);

    string getCommand (int value);

public:
    static string getTitle ();

    static string getValueStr (string name,
                               int    value
                               );

    static string BinaryOperator (string RK,
                                  int    addressOP2,
                                  int    addressOP3,
                                  int    valueOP2,
                                  int    valueOP3
                                  );

    static string getAddressToken (int address);

    static string getNiceCommand (int    address,
                                  string RK
                                  );
};
