/*
Заголовочный файл, описывающий
класс для работы с матрицей высот, используя
подгрузку части матрицы из файла в оперативную память.
Класс в этом файле описывает ячейку оперативной памяти.
Ячейка разделяемая. Если она уже подгружена, а в программе открывается
несколько раз файл BMH, то дополнительных загрузок не будет.
Ведется подсчет пользователей каждой ячейки для очистке при
ненужности.

Обновлен  11.08.2023
*/

#ifndef MEMORYITEM_H
#define MEMORYITEM_H
#include <ctype.h>
#include <map>
#include <mutex>

#include "Map/MString.h"
#include "DemsTable.h"
#include "Dem.h"

class MString;

class MemoryItem
{
    friend class Demming;
    friend class Memory;

private:

    MemoryItem(const BmhDescriptor &obj, const int &pageSide);

    void Clear();

    ~MemoryItem();

    void operator++();
    void operator--();

    unsigned usersCnt;
    DemsTable *memory;

    mutable std::mutex mu;
};


#endif // MEMORYITEM_H
