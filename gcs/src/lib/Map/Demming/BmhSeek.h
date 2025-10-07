/*
Заголовочный файл, описывающий
класс для работы с матрицей высот, используя
сдвиг текущей позиции. Это наиболее медленный способ,
но самый надежный.
Обновлен  11.08.2023
*/
#ifndef SEEKBMH_H
#define SEEKBMH_H

#include <qiodevice.h>

#include "MBmhFILE.h"
#include "BmhDescriptor.h"
#include "IValid.h"

class BmhSeek : public MBmhFILE
{
public:

    /*конструктор, newpath - путь файла матрицы высот*/
    BmhSeek(const MString &path);

    virtual ~BmhSeek() override;

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
    virtual bool IsValid() const override;

protected:

    virtual void Clear() override;

private:

    BmhDescriptor bmhDescr;

    QIODevice* pFile;
    mutable std::mutex mu;
};

#endif // SEEKBMH_H
