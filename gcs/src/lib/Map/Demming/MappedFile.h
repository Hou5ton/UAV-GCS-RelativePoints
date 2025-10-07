/*
Заголовочный файл описывающий класс,
инкапсулирующий отображение файла в
оперативную память и предоставляющий
интерфейс для работы.

Обновлен  11.08.2023
*/

#ifndef MAPPEDFILE_H
#define MAPPEDFILE_H

#include <QFile>

#include "Map/MString.h"

#include "BmhConstants.h"

#include "IValid.h"

class RAM;

class MappedFile : public IValid
{
    friend class BmhMap;
    friend class RAM;

private:

    MappedFile(const MString &path);
    virtual ~MappedFile();

    void Clear();

    void operator++();
    void operator--();

    const uchar* GetMap() const;

    bool IsValid() const override;

    unsigned UsersCnt() const;

    uchar* fileMemMap;
    unsigned usersCnt;

    QFileDevice* pFile;
};

#endif // MAPPEDFILE_H
