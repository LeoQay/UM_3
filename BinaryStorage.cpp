#pragma once

#include "BinaryStorage.h"

BinaryStorage::BinaryStorage(int size)
{
    this->size = 0;
}

BinaryStorage::~BinaryStorage()
{

}

void BinaryStorage::push(int value)
{
    mas[size++] = value;
}

int BinaryStorage::get(int index) const
{
    return mas[index];
}
