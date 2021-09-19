
#ifndef UM_3_CONVERTOR_H
#define UM_3_CONVERTOR_H

#include <string>

#include "BinaryStorage.h"

using namespace std;

class Convertor
{
public:

    /*
     * Function convert txt file with define format:
     *     Every length of line in file equal 32
     *     Every line ENDS with symbol '\n'
     *     Every symbol IN lines equal '0' or '1'
     * EXAMPLE:
     *     00000000000000000000000000000000
     *     11111000000000000000000000000000
     * END OF EXAMPLE
    */
    static string txt_to_bin(string file_name);

    static string bin_to_txt(string file_name);

private:

    static void bin_to_storage(string BinFile, BinaryStorage & storage);

    static void storage_to_bin(BinaryStorage & storage, string BinFile);

    static void txt_to_storage(string TxtFile, BinaryStorage & storage);

    static void storage_to_txt(BinaryStorage & storage, string TxtFile);

    static void source_text_to_storage(string SourceFile, BinaryStorage & storage);
};


#endif //UM_3_CONVERTOR_H
