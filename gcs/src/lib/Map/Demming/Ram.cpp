#include "Ram.h"

RAM::~RAM()
{
    for(auto &el : memory)
    {
        delete el.second;
        el.second = nullptr;
    }

    memory.clear();
}

const uchar* RAM::GetMemory(const MString &path)
{
    std::lock_guard<std::mutex> lck(mu);

    auto iter = memory.find(path);

    if(iter != memory.end())
    {
        iter->second->operator++();
        return iter->second->GetMap();
    }

    MappedFile *file =  new MappedFile(path);

    if(file && file->IsValid())
    {
        file->operator++();
        memory.insert({path, file});
        return file->GetMap();
    }
    else
    {
        delete  file;
    }

    return nullptr;
}

void RAM::ReturnMemory(const MString &path)
{
    std::lock_guard<std::mutex> lck(mu);

    auto iter = memory.find(path);

    if(iter == memory.end())
    {
        return;
    }

    iter->second->operator--();

    if(iter->second->UsersCnt() <= 0)
    {
        delete iter->second;
        memory.erase(iter);
    }
}


