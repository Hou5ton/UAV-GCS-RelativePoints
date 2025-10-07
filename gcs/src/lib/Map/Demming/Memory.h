/*
Заголовочный файл, описывающий
класс, который предоставляет виртуальную
машину для работы с оперативной памятью.
Принцип работы - как при страничном распределнии
виртуальной памяти в ОС.

Обновлен  11.08.2023
*/

#ifndef MEMORY_H
#define MEMORY_H

#include <map>
#include <mutex>

#include "Map/MString.h"

#include "Dem.h"
#include "MemoryItem.h"

class Memory
{
    friend class Demming;

private:

    Memory() = default;

    ~Memory();

    MemoryItem* GetItemToUse(const MString &str,
                             const BmhDescriptor &obj,
                             const int &pageSide);

    void ReturnItem(const MString &str);

    std::map<size_t, MemoryItem*> items;
    std::mutex mu;
};

#endif // MEMORY_H
