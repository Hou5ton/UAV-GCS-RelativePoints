#include "Memory.h"

Memory::~Memory()
{
    for(auto &el : items)
    {
        delete el.second;
        el.second = nullptr;
    }

    items.clear();
}

MemoryItem* Memory::GetItemToUse(const MString &str,
                                 const BmhDescriptor &obj,
                                 const int &pageSide)
{
    std::lock_guard<std::mutex> lck(mu);

    size_t hsh = std::hash<std::string>{}(str);

    auto iter = items.find(hsh);

    if(iter != items.end())
    {
        iter->second->operator++();
        return iter->second;
    }

    MemoryItem *newItem = new MemoryItem(obj, pageSide);
    items.insert({hsh, newItem});
    newItem->operator++();

    return newItem;
}

void Memory::ReturnItem(const MString &str)
{
    std::lock_guard<std::mutex> lck(mu);

    auto iter = items.find(std::hash<std::string>{}(str));

    if(iter == items.end())
    {
        return;
    }

    iter->second->operator--();

    if(iter->second->usersCnt <= 0)
    {
        delete iter->second;
        items.erase(iter);
    }
}
