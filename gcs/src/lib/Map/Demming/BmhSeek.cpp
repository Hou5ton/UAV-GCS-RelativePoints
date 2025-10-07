#include <QBuffer>

#include "Map/Id.h"
#include "Map/BaseCl.h"

#include "BmhSeek.h"
#include "BmhConstants.h"

BmhSeek::BmhSeek(const MString &path)
    : bmhDescr(BmhDescriptor(path))
{
    pFile = new QFile(path.c_str());

    if(pFile)
    {
        pFile->open(QIODevice::ReadOnly );
        pFile->seek(0);
    }
    else
    {
        IValid::SetValid(false);
        return;
    }
}

BmhSeek::~BmhSeek()
{
    //Clear();
}

int BmhSeek::GetHeight(const MPoint &pt) const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    if(!bmhDescr.Contain(pt))
    {
        return BMH_OUTOFBOUNDS;
    }

    short value = BMH_OUTOFBOUNDS;

    int CurSeek = ((static_cast<int>((pt.y -
                                      bmhDescr.bottom) /
                                     bmhDescr.step) * bmhDescr.mm +
                    static_cast<int>((pt.x - bmhDescr.left) /
                                     bmhDescr.step)) *
                   BmhConstants::shortSz) +
            BmhConstants::headerOffset;

    std::unique_lock<std::mutex> lck(mu);

    pFile->seek(CurSeek);
    pFile->read(reinterpret_cast<char*>(&value),
                BmhConstants::shortSz);

    lck.unlock();

    if(reverseReadFlag)
    {
        reverseValue(reinterpret_cast<char*>(&value),
                     BmhConstants::shortSz);
    }

    return (value == BMH_NOVALUE) ?
                BMH_OUTOFBOUNDS :
                value;
}

char *BmhSeek::GetHeightsRow(const MPoint &pt,
                             int rowSize,
                             int &bytesRead) const
{
    rowSize = BmhConstants::shortSz * rowSize;

    char* arr = new char[rowSize];

    if(!IsValid())
    {
        return arr;
    }

    int curSeek = ((static_cast<int>((pt.y - bmhDescr.bottom) /
                                     bmhDescr.step) * bmhDescr.mm +
                    static_cast<int>((pt.x - bmhDescr.left) /
                                     bmhDescr.step)) *
                   BmhConstants::shortSz) + BmhConstants::headerOffset;

    std::unique_lock<std::mutex> lck(mu);

    pFile->seek(curSeek);
    bytesRead = static_cast<int>(pFile->read(arr, rowSize));

    lck.unlock();

    if(reverseReadFlag)
    {
        reverseValue(arr, rowSize);
    }

    return arr;
}

int BmhSeek::MxM() const
{
    if(!IsValid())
    {
        return 0;
    }

    return bmhDescr.mm;
}

int BmhSeek::MxN() const
{
    if(!IsValid())
    {
        return 0;
    }

    return bmhDescr.mn;
}

double BmhSeek::Step() const
{
    if(!IsValid())
    {
        return 0;
    }

    return bmhDescr.step;
}

double BmhSeek::Left() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.left;
}

double BmhSeek::Right() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.right;
}

double BmhSeek::Bottom() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.bottom;
}

double BmhSeek::Top() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.top;
}

bool BmhSeek::IsValid() const
{
    return bmhDescr.IsValid() &&
            pFile &&
            pFile->isOpen() &&
            IValid::IsValid();
}

void BmhSeek::Clear()
{
    if (pFile && pFile->isOpen())
    {
        pFile->close();

        QString clName = pFile->metaObject()->className();

        if (pFile->metaObject()->className()==QString("QBuffer"))
        {
            delete  &reinterpret_cast<QBuffer*>(pFile)->data();
        }

        delete pFile;
        pFile = nullptr;
    }
}
