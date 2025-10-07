/*
Заголовочный файл, описывающий
класс для работы с матрицей высот, используя
оображение файла. Это наиболее быстрый способ.

Обновлен  11.08.2023

Автор - Климашевич Н. А.
*/
#ifndef BMHMAP_H
#define BMHMAP_H

#include <QFileDevice>
#include <mutex>

#include "Map/MString.h"

#include "MBmhFILE.h"
#include "BmhDescriptor.h"
#include "BmhConstants.h"
#include "Ram.h"

#include "IValid.h"

class BmhMap : public MBmhFILE
{
public:

    /*конструктор, newpath - путь файла матрицы высот*/
    BmhMap(const MString &newpath);

    virtual ~BmhMap() override;

    /*Получение высоты в данной географической точке*/
    virtual int GetHeight(const MPoint &pt) const override;

    /*получение ряда высот*/
    virtual char *GetHeightsRow(const MPoint &pt,
                                int rowSize,
                                int &bytesRead) const override;

    /*количество столбцов матрицы высот*/
    virtual int MxM() const override;

    /*количество строк матрицы высот*/
    virtual int MxN() const override;

    /*шаг матрицы высот*/
    virtual double Step() const override;

    /*географическая долгота левого края области матрицы высот*/
    virtual double Left() const override;

    /*географическая долгота правого края области матрицы высот*/
    virtual double Right() const override;

    /*географическая широта нижнего края области матрицы высот*/
    virtual double Bottom() const override;

    /*географическая широта нижнего края области матрицы высот*/
    virtual double Top() const override;

    /*файл валиден, елси удалось
      прочитать информацию о нем
     отобразить данные в память*/
    bool IsValid() const override;

protected:

    virtual void Clear() override;

private:

    MString path;
    const uchar* fileMemMap;

    BmhDescriptor bmhDescr;

    static RAM memory;
};

#endif // BMHMAP_H
