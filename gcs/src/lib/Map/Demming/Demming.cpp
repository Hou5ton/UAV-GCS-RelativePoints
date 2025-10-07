#include <QBuffer>

#include "Map//BaseCl.h"
#include "Map//Refglobal.h"
#include "Map//MString.h"
#include "Map/Id.h"

#include "Demming.h"
#include "Dem.h"
#include "BmhConstants.h"

#ifdef _WIN32 || _WIN64
#include <windows.h>
#endif

#ifdef linux
#include <unistd.h>
#endif

Memory Demming::memory;

Demming::Demming(const MString &newPath)
    : path(newPath), bmhDescr(newPath)
{
    SetPagesSz();

    if((totalPages > ((bmhDescr.mm / pageSide) *
                      (bmhDescr.mn / pageSide))) ||
            (totalPages < 1))
    {
        Clear();

        IValid::SetValid(false);
        return;
    }

    pFile = new QFile(path.c_str());

    if(pFile)
    {
        pFile->open(QIODevice::ReadOnly);
        pFile->seek(0);
    }
    else
    {
        IValid::SetValid(false);
        return;
    }

    item = memory.GetItemToUse(newPath, bmhDescr, pageSide);
    table = item->memory;
}

Demming::~Demming()
{
    //Clear();
}


void Demming::Clear()
{
    bmhDescr.Clear();
    memory.ReturnItem(path);
    table = nullptr;

    if (pFile && pFile->isOpen())
    {
        pFile->close();

        QString clName = pFile->metaObject()->className();

        if (pFile->metaObject()->className() == QString("QBuffer"))
        {
            delete & reinterpret_cast<QBuffer*>(pFile)->data();
        }

        delete pFile;
    }
}

int Demming::GetHeight(const MPoint &point) const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    if(!bmhDescr.Contain(point))
    {
        return BMH_OUTOFBOUNDS;
    }

    int partI = static_cast<int>((point.x - bmhDescr.left) /
                                 bmhDescr.step) / pageSide;


    int partJ = static_cast<int>((point.y - bmhDescr.bottom) /
                                 bmhDescr.step) / pageSide;


    int indJinPart = static_cast<int>((point.y - bmhDescr.bottom) /
                                      bmhDescr.step) % pageSide;


    int indIinPart = static_cast<int>((point.x - bmhDescr.left) /
                                      bmhDescr.step) % pageSide;


    if(!table->dems[partJ][partI]->IsValid())
    {
        return BMH_OUTOFBOUNDS;
    }

    std::lock_guard<std::mutex> lck(item->mu);

    if(table->dems[partJ][partI])
    {
        return table->dems[partJ][partI]->part[indJinPart][indIinPart];
    }

    table->ClearRandomCell(totalPages);

    LoadPart(pageSide,
             partI,
             partJ);

    return table->dems[partJ][partI]->part[indJinPart][indIinPart];
}

char* Demming::GetHeightsRow(const MPoint &point,
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

    int sz = (bmhDescr.fileSz >= curPos +
              rowSize) ?  rowSize :
                          (static_cast<int>(bmhDescr.fileSz - curPos));

    uchar* fileMemMap = pFile->map(curPos + BmhConstants::headerOffset, sz);

    if(memcpy(arr, fileMemMap, static_cast<size_t>(sz)))
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

    pFile->unmap(fileMemMap);

    return arr;
}

unsigned Demming::Pairing(const int &k1, const int &k2) const
{
    return (k1 + k2) * (k1 + k2 + 1) / 2 + k2;
}

int Demming::MxM() const
{
    if(!IsValid())
    {
        return 0;
    }

    return bmhDescr.mm;
}

int Demming::MxN() const
{
    if(!IsValid())
    {
        return 0;
    }

    return bmhDescr.mn;
}

double Demming::Step() const
{
    if(!IsValid())
    {
        return 0;
    }

    return bmhDescr.step;
}

double Demming::Left() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.left;
}

double Demming::Right() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.right;
}

double Demming::Bottom() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.bottom;
}

double Demming::Top() const
{
    if(!IsValid())
    {
        return BmhConstants::NO_DATA;
    }

    return bmhDescr.top;
}

bool Demming::IsValid() const
{
    return  IValid::IsValid() &&
            item &&
            table &&
            bmhDescr.IsValid() &&
            pFile &&
            pFile->isOpen();
}

void Demming::SetPagesSz()
{
    unsigned long long swappingSz =
            static_cast<unsigned long long>(bmhDescr.fileSz * PART_OF_FILE);

    unsigned long long maxSwappingSz = 0;


#ifdef _WIN32 || _WIN64
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    maxSwappingSz = status.ullTotalPhys * PART_OF_RAM;
#endif

#ifdef linux
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    maxSwappingSz = pages * page_size * PART_OF_RAM;
#endif

    if(maxSwappingSz == 0)
    {
        maxSwappingSz = 33554432ULL; //~ 32MB -> def max sz
    }

    if(swappingSz > maxSwappingSz)
    {
        swappingSz = maxSwappingSz;
    }

    swappingSz /= 2;
    swappingSz /= PAGES_PER_SIDE;

    if(static_cast<unsigned long long>(std::pow(swappingSz, 1.0 / 3.0) * PAGES_PER_SIDE) >
            static_cast<unsigned long long>(INT_MAX))
    {
        totalPages = INT_MAX - 1;
        pageSide = (INT_MAX - 1) / PAGES_PER_SIDE;
    }
    else
    {
        pageSide = static_cast<int>(std::pow(swappingSz, 1.0 / 3.0));
        totalPages = pageSide * PAGES_PER_SIDE;
    }

    if(!pageSide || !totalPages)
    {
        pageSide = (bmhDescr.mm > bmhDescr.mn) ?
                    bmhDescr.mn : bmhDescr.mm;

        totalPages = PAGES_PER_SIDE;
    }
}

void Demming::LoadPart(const int &pageSide,
                       const int &partI,
                       const int &partJ) const
{
    Dem* area = new Dem(pageSide);

    table->dems[partJ][partI] = area;

    int curPos = BmhConstants::headerOffset +
            (partJ * bmhDescr.mm *
             pageSide +
             partI * pageSide) *
            BmhConstants::shortSz;

    for(int i = 0; i < pageSide &&
        curPos < bmhDescr.fileSz; ++i)
    {
        auto sz = (bmhDescr.fileSz >= curPos +
                   pageSide * BmhConstants::shortSz) ?
                    pageSide * BmhConstants::shortSz :
                    (bmhDescr.fileSz - curPos);

        uchar *fileMemMap = pFile->map(curPos, sz);

        memcpy(area->part[i], fileMemMap,
               static_cast<size_t>(sz));

        pFile->unmap(fileMemMap);

        curPos += bmhDescr.mm * BmhConstants::shortSz;
    }
}
