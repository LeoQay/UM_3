#include <fstream>
#include <string>

#include "Convertor.h"
#include "Exception.h"
#include "Tools.h"
#include "Translator.h"
#include "Memory.h"

void Convertor::txt_to_bin(string TxtFile, string OutFile)
{
    ifstream fin(TxtFile);

    if (!fin.is_open())
    {
        fin.close();
        throw Exception("File " + TxtFile + " doesn't exist");
    }

    fin.close();

    BinaryStorage buffer;

    if (OutFile.empty()) {
        OutFile = TxtFile + ".bin";
    }

    txt_to_storage(TxtFile, buffer);
    storage_to_bin(buffer, OutFile);
}


void Convertor::bin_to_txt(string BinFile, string OutFile)
{
    fstream fin(BinFile, ios::binary|ios::in);

    if (!fin.is_open())
    {
        throw Exception("File " + BinFile + " doesn't exist");
    }

    BinaryStorage buffer;

    if (OutFile.empty()) {
        OutFile = BinFile + ".txt";
    }

    bin_to_storage(BinFile, buffer);
    storage_to_txt(buffer, OutFile);
}


void Convertor::bin_to_storage(string  BinFile, BinaryStorage & storage)
{
    fstream fin(BinFile, ios::binary|ios::in);

    if (!fin.is_open())
    {
        throw Exception("File " + BinFile + " doesn't exist");
    }


    int current;

    fin.read((char*)&current, sizeof(int));

    while (!fin.eof())
    {
        storage.push(current);

        fin.read((char*)&current, sizeof(int));
    }


    fin.close();
}


void Convertor::storage_to_bin(BinaryStorage & storage, string BinFile)
{
    fstream fin(BinFile, ios::binary|ios::out);

    if (!fin.is_open())
    {
        throw Exception("File " + BinFile + " doesn't created");
    }

    fin.clear();

    for (int i = 0; i < storage.size; i++)
    {
        int current = storage.get(i);

        fin.write((char*)&current, sizeof(int));
    }

    fin.close();
}


void Convertor::txt_to_storage(string TxtFile, BinaryStorage & storage)
{
    ifstream fin(TxtFile);

    if (!fin.is_open())
    {
        throw Exception("File " + TxtFile + " doesn't exist");
    }

    while (!fin.eof())
    {
        string cell;
        fin >> cell;

        if (!cell.empty())
        {
            bool all_is_good = true;

            if (cell.length() != 32) all_is_good = false;

            for (int i = 0; i < 32; i++)
            {
                if (!all_is_good || cell[i] != '0' && cell[i] != '1') all_is_good = false;
            }

            if (!all_is_good)
            {
                fin.close();
                throw Exception("Bad format of punched card txt file");
            }

                int value = Tools::StrToInt(cell);

                storage.push(value);
        }else
            break;
    }

    fin.close();
}


void Convertor::storage_to_txt(BinaryStorage & storage, string TxtFile)
{
    ofstream fout(TxtFile);

    for (int i = 0; i < storage.size; i++)
    {
        fout << Tools::IntToStr(storage.get(i)) << endl;
    }

    fout.close();
}


void Convertor::source_text_to_storage(string SourceFile, BinaryStorage & storage)
{
    Memory* memory = new Memory;

    int size = Translator::Translate(SourceFile, *memory);

    for (int i = 1; i <= size; i++) storage.push(memory->getInt(i));

    delete memory;
}


void Convertor::storage_to_memory(BinaryStorage & storage, Memory & memory, int first)
{
    for (int i = 0; i < storage.size; i++)
    {
        memory.push(i + first, storage.get(i));
    }
}


void Convertor::punched_card_bin_to_memory(string punched_card, Memory & memory)
{
    BinaryStorage * storage = new BinaryStorage;

    bin_to_storage(punched_card, *storage);

    storage_to_memory(*storage, memory, 1);

    delete storage;
}

void Convertor::punched_card_source_to_bin(string SourceFile, string BinFile)
{
    BinaryStorage * storage = new BinaryStorage;

    if(BinFile == "") {
        BinFile = SourceFile.substr(0, SourceFile.find(".txt")) + ".bin";
    }

    source_text_to_storage(SourceFile, *storage);

    storage_to_bin(*storage, BinFile);

    delete storage;
}
