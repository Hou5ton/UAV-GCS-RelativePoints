/*
Заголовочный файл, описывающий
класс для работы с матрицей высот, используя
подгрузку части матрицы из файла в оперативную память.
Принцип работы - как при страничном распределнии
виртуальной памяти в ОС.
Обновлен  11.08.2023
*/

#ifndef DEMMING_H
#define DEMMING_H

#include <climits>
#include <cmath>
#include <mutex>

#include "MBmhFILE.h"
#include "BmhDescriptor.h"

#include "Map/MString.h"

#include "MemoryItem.h"
#include "Memory.h"

#include "IValid.h"

/*класс реализует общий интерфейс работы
с BMH-файлами*/
class Demming : public MBmhFILE
{

public:

    /*конструктор, newpath - путь файла матрицы высот*/
    Demming(const MString &newPath);

    virtual ~Demming();

    /*Получение высоты в данной географической точке*/
    virtual int GetHeight(const MPoint &pt) const override;

    /*получение ряда высот*/
    virtual char* GetHeightsRow(const MPoint &pt,
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

    void SetPagesSz();

    /*каждая подгружаемая часть храниться
      в map, при попытке получить высоту
      из XY координат, определяются XY подгружаемой
      в память матрицы и данной функцией преобразуются
      в уникальное число, которое является
      ключом для карты*/
    unsigned Pairing(const int &k1,
                     const int &k2) const;

    void LoadPart(const int &pageSide,
                  const int &partI,
                  const int &partJ) const;

    MString path;
    QFile* pFile;

    int pageSide;
    int totalPages;
    //double stepMultSide;

    MemoryItem *item;
    DemsTable* table;
    BmhDescriptor bmhDescr;

    static Memory memory;

    static constexpr double PART_OF_RAM = 1 / 32;

    static constexpr double PART_OF_FILE = 7 / 24;

    static constexpr int PAGES_PER_SIDE = 1;

    static constexpr int DEF_PAGE_SZ = 128;
    static constexpr int DEF_PAGES_NUM = 128;
};

#endif // DEMMING_H
