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
    string getTitle ();

    string getValueStr (string name,
                        int    value
                        );
};
