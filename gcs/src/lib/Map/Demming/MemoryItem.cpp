#include "MemoryItem.h"

MemoryItem::MemoryItem(const BmhDescriptor &obj,
                       const int &pageSide)

    : usersCnt(0), memory(nullptr)
{
    if(memory)
    {
        Clear();
    }

    memory = new DemsTable(obj, pageSide);
}

void MemoryItem::Clear()
{
    delete  memory;
    memory = nullptr;
}


MemoryItem::~MemoryItem()
{
    Clear();
}

void MemoryItem::operator++()
{
    ++usersCnt;
}

void MemoryItem::operator--()
{
    --usersCnt;
}
