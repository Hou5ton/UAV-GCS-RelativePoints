/*
Заголовочный файл, описывающий
характеристики матрицы высот
(географические границ, количество строк и столбцов)
из файла BMH.

Обновлен  11.08.2023

Автор - Климашевич Н. А.
*/
#ifndef BMHDESCRIPTOR_H
#define BMHDESCRIPTOR_H

#include <string>
#include <stdexcept>

#include <QFile>
#include <QFileDevice>

#include "Map/MString.h"
#include "Map/Refglobal.h"

#include "IValid.h"

class MPoint;

/*Дескриптор файла матрицы высот.
 Класс наследует IValid установки
 и проверки корректности класса*/
struct BmhDescriptor : public IValid
{
    BmhDescriptor(const MString &path);

    ~BmhDescriptor();

    BmhDescriptor(const BmhDescriptor &obj);

    /*Дескриптов валиден, если
    получилось прочитать описание матрицы*/
    bool IsValid() const override;

    void Clear();

    void NormalizeBoundaries();

    /*Проверка содержания точки в
     границах территории матрицы высот*/
    bool Contain(const MPoint &pt) const;

    /*верхняя граница территории для которой
      есть матрица высот*/
    double	top;

    /*нижняя граница территории для которой
      есьт матрица высот*/
    double	bottom;

    /*левая граница территории для которой
      есьт матрица высот*/
    double	left;

    /*правая граница территории для которой
      есьт матрица высот*/
    double	right;

    /*число столбцов и строк матрицы*/
    int mn;
    int mm;

    /*шаг матрицы высот*/
    double step;

    /*размер описываемого файла матрицы высот*/
    qint64 fileSz;
};

#endif // BMHDESCRIPTOR_H
