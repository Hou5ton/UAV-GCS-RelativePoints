#include "Map/BaseCl.h"
#include "Map/Refglobal.h"

#include "BmhConstants.h"
#include "BmhDescriptor.h"

BmhDescriptor::BmhDescriptor(const MString &path)
{
    auto* pFile = new QFile(path.c_str());

    if (pFile)
    {
        pFile->open(QIODevice::ReadOnly);
        pFile->seek(0);        
    }
    else
    {
        IValid::SetValid(false);
        return;
    }

    /*пытаемся считать заголовок, используя отображение файла*/
    uchar *fileMap = pFile->map(0, BmhConstants::headerOffset);

    if(fileMap)
    {
        memcpy(&left, fileMap + sizeof (double) * 0,
               sizeof (double));


        memcpy(&bottom, fileMap + sizeof (double) * 1,
               sizeof (double));


        memcpy(&right, fileMap + sizeof (double) * 2,
               sizeof (double));


        memcpy(&top, fileMap + sizeof (double) * 3,
               sizeof (double));


        memcpy(&mn, fileMap +
               (sizeof (double) * BmhConstants::RECT_SIDES),
               sizeof (int));


        memcpy(&mm, fileMap +
               (sizeof (double) * BmhConstants::RECT_SIDES) +
               sizeof (int),
               sizeof (int));


        memcpy(&step, fileMap +
               (sizeof (double) * BmhConstants::RECT_SIDES) +
               sizeof (int) * 2,
               sizeof (double));


        if(reverseReadFlag)
        {
            reverseValue(reinterpret_cast<char*>(&left),
                         sizeof (left));

            reverseValue(reinterpret_cast<char*>(&bottom),
                         sizeof (bottom));

            reverseValue(reinterpret_cast<char*>(&right),
                         sizeof (right));

            reverseValue(reinterpret_cast<char*>(&top),
                         sizeof (top));

            reverseValue(reinterpret_cast<char*>(&mn),
                         sizeof (mn));

            reverseValue(reinterpret_cast<char*>(&mm),
                         sizeof (mm));

            reverseValue(reinterpret_cast<char*>(&step),
                         sizeof (step));
        }
    }
    else
    {
        Clear();
        IValid::SetValid(false);
        return;
    }

    fileSz = pFile->size();
    NormalizeBoundaries();
    pFile->unmap(fileMap);

    pFile->close();

    delete pFile;
    pFile = nullptr;
}

BmhDescriptor::BmhDescriptor(const BmhDescriptor &obj)
{
    this->top = obj.top;
    this->bottom = obj.bottom;
    this->left = obj.left;
    this->right = obj.right;

    this->mn = obj.mn;
    this->mm = obj.mm;

    this->step = obj.step;

    this->fileSz = obj.fileSz;
}

BmhDescriptor::~BmhDescriptor()
{
    Clear();
}

void BmhDescriptor::Clear()
{
    top = 0;
    bottom = 0;
    left = 0;
    right = 0;

    mn = 0;
    mm = 0;

    step  = 0;
    fileSz = 0;
}

void BmhDescriptor::NormalizeBoundaries()
{
    if (left > right)
    {
        std::swap(left, right);
    }

    if (top < bottom)
    {
        std::swap(top, bottom);
    }
}

bool BmhDescriptor::Contain(const MPoint &pt) const
{
    return (pt.x > left &&
            pt.x < right &&
            pt.y > bottom &&
            pt.y < top) && IsValid();
}

bool BmhDescriptor::IsValid() const
{
    return IValid::IsValid();
}
