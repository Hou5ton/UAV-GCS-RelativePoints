#include "Map/BaseCl.h"
#include "Map/Refglobal.h"
#include "Map/MString.h"
#include "Map/Id.h"

#include "BmhMap.h"
#include "BmhConstants.h"

RAM BmhMap::memory;

BmhMap::BmhMap(const MString &newpath)

    : path(newpath),
      fileMemMap(memory.GetMemory(newpath)),
      bmhDescr(BmhDescriptor(newpath))
{
    if(!IsValid())
    {
        return;
    }

    fileMemMap += BmhConstants::headerOffset;
}

BmhMap::~BmhMap()
{
    //Clear();
}

void BmhMap::Clear()
{    
    memory.ReturnMemory(path);
    fileMemMap = nullptr;
    bmhDescr.Clear();
}

int BmhMap::GetHeight(const MPoint &pt) const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    short height = BMH_OUTOFBOUNDS;

    if(!bmhDescr.Contain(pt))
    {
        return height;
    }

    int curPos = ((static_cast<int>((pt.y -
                                     bmhDescr.bottom) /
                                    bmhDescr.step) * bmhDescr.mm +
                   static_cast<int>((pt.x - bmhDescr.left) /
                                    bmhDescr.step)) *
                  BmhConstants::shortSz);

    if(fileMemMap == nullptr)
    {
        int nymrht = 0;
        ++nymrht;
    }

    memcpy(&height, fileMemMap + curPos, sizeof(height));

    if(reverseReadFlag)
    {
        reverseValue(reinterpret_cast<char*>(&height),
                     sizeof(height));
    }

    if(height == BMH_NOVALUE)
    {
        return BMH_OUTOFBOUNDS;
    }

    return  height;
}

char* BmhMap::GetHeightsRow(const MPoint &point,
                            int rowSize,
                            int &bytesRead) const
{
    if(static_cast<unsigned long long>(rowSize) >=
            BmhConstants::MAX_FILE_SZ)
    {
        return nullptr;
    }

    rowSize *= BmhConstants::shortSz;

    char* arr = new char[rowSize];

    if(!IsValid())
    {
        return arr;
    }

    int curPos = ((static_cast<int>((point.y - bmhDescr.bottom) /
                                    bmhDescr.step) * bmhDescr.mm +
                   static_cast<int>((point.x - bmhDescr.left) /
                                    bmhDescr.step)) * BmhConstants::shortSz);

    int sz = (bmhDescr.fileSz > curPos +
              rowSize) ?
                rowSize :
                (static_cast<int>(bmhDescr.fileSz - curPos));

    if(memcpy(arr, fileMemMap + curPos,
              static_cast<size_t>(sz)))
    {
        bytesRead =  sz;
    }
    else
    {
        bytesRead = -1;
    }

    if(reverseReadFlag)
    {
        reverseValue(arr, static_cast<int>(sz));
    }

    return arr;
}

bool BmhMap::IsValid() const
{
    return fileMemMap != nullptr &&
            bmhDescr.IsValid() && IValid::IsValid();
}

int BmhMap::MxM() const
{
    if(!IsValid())
    {
        return 0;
    }

    return bmhDescr.mm;
}

int BmhMap::MxN() const
{
    if(!IsValid())
    {
        return 0;
    }

    return bmhDescr.mn;
}

double BmhMap::Step() const
{
    if(!IsValid())
    {
        return 0;
    }

    return bmhDescr.step;
}

double BmhMap::Left() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.left;
}

double BmhMap::Right() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.right;
}

double BmhMap::Bottom() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.bottom;
}

double BmhMap::Top() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.top;
}
