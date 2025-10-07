#include "Map/Id.h"

#include "Dem.h"
#include "BmhConstants.h"

Dem::Dem(const int &side)
    : MN(0),
      part(nullptr)
{
    ReInit(side);
}

Dem::~Dem()
{
    Clear();
}

void Dem::ReInit(const int &mxn)
{
    Clear();

    if(mxn < 1)
    {
        IValid::SetValid(false);;
        return;
    }

   // Clear();

    MN = mxn;

    part  = new short* [MN];

    for(int j = 0; j < MN; ++j)
    {
        part[j] = new short[MN];

        for(int k = 0; k < MN; ++k)
        {
            part[j][k] = BMH_OUTOFBOUNDS;
        }
    }
}

void Dem::Clear()
{
    for(int i = 0; i < MN; ++i)
    {
        delete part[i];
    }

    delete part;

    part = nullptr;
    MN = 0;
}

bool Dem::IsValid() const
{
    return IValid::IsValid() && part;
}
