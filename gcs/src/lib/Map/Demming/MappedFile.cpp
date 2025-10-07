#include "MappedFile.h"

MappedFile::MappedFile(const MString &path)

    : fileMemMap(nullptr),
      usersCnt(0),
      pFile(nullptr)
{
    IValid::SetValid(false);

    pFile = new QFile(path.c_str());

    if (pFile)
    {
        pFile->open(QIODevice::ReadOnly);
        pFile->seek(0);
    }
    else
    {
        return;
    }

    if(static_cast<unsigned long long>(pFile->size())
            >= BmhConstants::MAX_FILE_SZ)
    {
        Clear();
        return;
    }

    fileMemMap = pFile->map(0, pFile->size());

    if(!fileMemMap)
    {
        Clear();
        return;
    }

    IValid::SetValid(true);
}

MappedFile::~MappedFile()
{
    Clear();
}

void MappedFile::Clear()
{
    if(pFile || fileMemMap)
    {
        pFile->unmap(fileMemMap);
        fileMemMap = nullptr;
    }

    if(pFile)
    {
        pFile->close();
        delete pFile;
    }

    pFile = nullptr;
    IValid::SetValid(false);
}

void MappedFile::operator++()
{
    if(IsValid())
        ++usersCnt;
}

void MappedFile::operator--()
{
    if(IsValid())
        --usersCnt;
}

const uchar* MappedFile::GetMap() const
{
    if(IsValid())
        return fileMemMap;

    return 0;
}

bool MappedFile::IsValid() const
{
    return IValid::IsValid();
}

unsigned MappedFile::UsersCnt() const
{
    return usersCnt;
}
