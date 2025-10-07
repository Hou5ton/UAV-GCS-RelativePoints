/*
Заголовочный файл, описывающий общий интерфейс
для работы с файлом матрицы высот.
Обновлен  11.08.2023
*/
#ifndef BMHENVELOPE_H
#define BMHENVELOPE_H

#include <string>
#include <mutex>

#include "IValid.h"

class MString;
class MPoint;

class MBmhFILE : public IValid
{

public:

    MBmhFILE(const MString &path);

    virtual ~MBmhFILE();

    /*Получение высоты в данной географической точке*/
    virtual int GetHeight(const MPoint &pt) const;

    /*получение ряда высот*/
    virtual char* GetHeightsRow(const MPoint &pt,
                                int rowSize,
                                int &bytesRead) const;

    /*количество столбцов матрицы высот*/
    virtual int MxM() const;

    /*количество строк матрицы высот*/
    virtual int MxN() const;

    /*шаг матрицы высот*/
    virtual double Step() const;

    /*географическая долгота левого края области матрицы высот*/
    virtual double Left() const;

    /*географическая долгота правого края области матрицы высот*/
    virtual double Right() const;

    /*географическая широта нижнего края области матрицы высот*/
    virtual double Bottom() const;

    /*географическая широта нижнего края области матрицы высот*/
    virtual double Top() const;

    /*файл валиден, елси удалось
      прочитать информацию о нем
     отобразить данные в память*/
    virtual bool IsValid() const;

protected:

    virtual void Clear();

    MBmhFILE() : pLetter(nullptr) {}

private:

    MBmhFILE(const MBmhFILE&);
    MBmhFILE &operator=(MBmhFILE&);
    MBmhFILE &operator=(const MBmhFILE&);

    MBmhFILE *pLetter;
};

#endif // BMHENVELOPE_H
