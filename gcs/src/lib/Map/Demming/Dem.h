/*
Заголовочный файл, описывающий
класс матрицы высот заданного программистом размера.
Класс используется при реализации подгрузки
части матрицы, содержит в себе часть матрицы BMH и
загружается в оперативную память из
файла для последующего быстрого использования.
Принцип работы - как при страничном распределении
виртуальной памяти в ОС.

Обновлен  11.08.2023
*/

#ifndef DEM_H
#define DEM_H

#include <stdexcept>

#include "IValid.h"

struct Dem : public IValid
{
    Dem(const int &side);

    ~Dem();

    void ReInit(const int &mxn);

    void Clear();

    int MN;
    short **part;

    bool IsValid() const override;
};

#endif // DEM_H
