#ifndef UM_3_BINARYSTORAGE_H
#define UM_3_BINARYSTORAGE_H


class BinaryStorage
{
public:
    int mas[512];
    int size;

    void push(int value);

    int get(int index) const;

    explicit BinaryStorage(int size = 512);

    ~BinaryStorage();
};


#endif //UM_3_BINARYSTORAGE_H
