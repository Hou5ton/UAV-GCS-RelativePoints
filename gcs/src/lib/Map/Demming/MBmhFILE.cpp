#include <cassert>

#include "Map/BaseCl.h"
#include "Map/Refglobal.h"
#include "Map/MString.h"

#include "MBmhFILE.h"
#include "BmhMap.h"
#include "BmhSeek.h"
#include "Demming.h"
#include "BmhConstants.h"

MBmhFILE::MBmhFILE(const MString &path)
    : pLetter(nullptr)
{
    if(BmhConstants::intSz != 4 ||
            BmhConstants::dblSz != 8)
    {
        IValid::SetValid(false);
        return;
    }

    pLetter = new BmhMap(path);

    if(!pLetter || !pLetter->IsValid())
    {
        delete  pLetter;
        pLetter = nullptr;
        pLetter = new Demming(path);
    }

    if(!pLetter || !pLetter->IsValid())
    {
        delete  pLetter;
        pLetter = nullptr;
        pLetter = new BmhSeek(path);
    }

    if(!pLetter || !pLetter->IsValid())
    {
        IValid::SetValid(false);
    }
}

MBmhFILE::~MBmhFILE()
{
    if (pLetter)
    {
        Clear();
    }

    delete pLetter;
    pLetter = nullptr;
}

void MBmhFILE::Clear()
{
    pLetter->Clear();
}

char* MBmhFILE::GetHeightsRow(const MPoint &pt,
                              int rowSize,
                              int &bytesRead) const
{
    if(!IsValid())
    {
        return 0;
    }

    return pLetter->GetHeightsRow(pt,
                                  rowSize,
                                  bytesRead);
}

int MBmhFILE::GetHeight(const MPoint &pt) const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return pLetter->GetHeight(pt);
}

int MBmhFILE::MxM() const
{
    if(!IsValid())
    {
        return 0;
    }

    return pLetter->MxM();
}

int MBmhFILE::MxN() const
{
    if(!IsValid())
    {
        return 0;
    }

    return pLetter->MxN();
}

double MBmhFILE::Step() const
{
    if(!IsValid())
    {
        return 0;
    }

    return pLetter->Step();
}

double MBmhFILE::Left() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return pLetter->Left();
}

double MBmhFILE::Right() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return pLetter->Right();
}

double MBmhFILE::Bottom() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return pLetter->Bottom();
}

double MBmhFILE::Top() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return pLetter->Top();
}

bool MBmhFILE::IsValid() const
{
    return IValid::IsValid() && pLetter->IsValid();
}
