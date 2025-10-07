/*
Заголовочный файл, описывающий
таблицу подгружаемых в память частей матрицы из BMH.
Принцип работы - как при страничном распределнии
виртуальной памяти в ОС. При подгрузке выгружается
спонтанная подматрица.

Обновлен  11.08.2023
*/

#ifndef DEMSTABLE_H
#define DEMSTABLE_H

#include <random>

#include "Dem.h"
#include "BmhDescriptor.h"

class DemsTable
{

public:

    DemsTable(const BmhDescriptor &obj, const int &pageSide);

    ~DemsTable();

    void ClearRandomCell(const int  &totalPages);

    //private:

    int i;
    int j;

    Dem*** dems;
};

#endif // DEMSTABLE_H
