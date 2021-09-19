#pragma once

#include <fstream>
#include <iostream>

#include "Convertor.h"
#include "Exception.h"
#include "Tools.h"
#include "Translator.h"
#include "Memory.h"

string Convertor::txt_to_bin(string TxtFile)
{
    ifstream fin(TxtFile);

    if (!fin.is_open())
    {
        fin.close();
        throw Exception("File " + TxtFile + " doesn't exist");
    }

    fin.close();

    string BinFile = TxtFile.substr(0, TxtFile.find(".txt")) + ".bin";

    BinaryStorage buffer;

    txt_to_storage(TxtFile, buffer);
    storage_to_bin(buffer, BinFile);

    return BinFile;
}


string Convertor::bin_to_txt(string BinFile)
{
    fstream fin(BinFile, ios::binary|ios::in);

    if (!fin.is_open())
    {
        throw Exception("File " + BinFile + " doesn't exist");
    }

    string TxtFile = BinFile.substr(0, BinFile.find(".bin")) + ".txt";

    BinaryStorage buffer;

    bin_to_storage(BinFile, buffer);
    storage_to_txt(buffer, TxtFile);

    return TxtFile;
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
        cout << current << "\n";
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

    storage.push(0);
    for (int i = 1; i <= size; i++) storage.push(memory->getInt(i));

    delete memory;
}
