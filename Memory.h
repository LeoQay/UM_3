struct Cell
{
    std::string val;
    bool empty;
};

class Memory
{
private:
    int mem_size = 512;
    Cell mem_mas[512];
public:
    Memory();

    std::string get(int index);

    void push(int index, std::string new_val);

    std::string outMemory();
};
