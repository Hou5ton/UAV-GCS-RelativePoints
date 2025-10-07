/*
Заголовочный файл, описывающий
класс для работы с матрицей высот. Данный класс
использует разделяемые отображения. То есть
при попытке открыть несколько раз один и тот же файл
в программе, будет использовано чтение готового отображения,
без лишних прогрузок в память.

Обновлен  11.08.2023
*/

#ifndef RAM_H
#define RAM_H

#include <mutex>

#include <QChar>

#include "Map/MString.h"

#include "MappedFile.h"

class RAM
{
    friend class BmhMap;

private:

    RAM() = default;
    ~RAM();

    const uchar* GetMemory(const MString &path);
    void ReturnMemory(const MString &path);

    std::map<MString, MappedFile*> memory;
    std::mutex mu;
};

#endif // RAM_H
