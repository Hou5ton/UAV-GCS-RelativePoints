#include "DemsTable.h"

DemsTable::DemsTable(const BmhDescriptor &obj, const int &pageSide)
{
    i = (static_cast<int>((obj.right - obj.left) /
                          obj.step) / pageSide) + 1;

    j = (static_cast<int>((obj.top - obj.bottom) /
                          obj.step) / pageSide) + 1;

    dems = new Dem**[j];

    for(int k = 0; k < j; ++k)
    {
        dems[k] = new Dem*[i];

        for(int z = 0; z < i; ++z)
        {
            dems[k][z] = nullptr;
        }
    }
}

DemsTable::~DemsTable()
{
    for(int k = 0; k < j; ++k)
    {
        for(int z = 0; z < i; ++z)
        {
            delete dems[k][z];
        }

        delete  dems[k];
    }

    delete  dems;
}

void DemsTable::ClearRandomCell(const int  &totalPages)
{
    std::srand(0);

    int ii = std::rand() % i;
    int jj = std::rand() % j;

    for(int k = 0; k < j*i / totalPages + 1; ++k)
    {
        if(dems[jj][ii])
        {
            delete dems[jj][ii];
            dems[jj][ii] = nullptr;
            return;
        }

        ii = std::rand() % i;
        jj = std::rand() % j;
    }
}
