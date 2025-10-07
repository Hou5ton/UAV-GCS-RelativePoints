/*
Заголовочный файл, описывающий константы
для работы с матрицей высот.

Обновлен  11.08.2023

Автор - Климашевич Н. А.
*/

#ifndef BMHCONSTANTS_H
#define BMHCONSTANTS_H

#include <QChar>
#include <climits>

namespace BmhConstants
{

/*вовзращаемое значение в случае неккоректной структуры/операции*/
constexpr int NO_DATA = 9999;

constexpr int RECT_SIDES = 4;

constexpr int dblSz = sizeof(double);

constexpr int intSz = sizeof(int);

constexpr int shortSz = sizeof(short);

/*смещение в байтах от начала файла BMH до начала координат*/
constexpr int headerOffset = (dblSz *  RECT_SIDES +
                              intSz * 2 + dblSz);

static constexpr unsigned long long BITS_IN_BYTE = 8;

/*определение максимального байта для индексации в данной системе*/
static unsigned long long GetMaxFileSz()
{
    if(sizeof(void*) * BITS_IN_BYTE >= BITS_IN_BYTE * BITS_IN_BYTE)
    {
        return ULLONG_MAX;
    }

    return 2ULL << ((sizeof (uchar*) * BITS_IN_BYTE));
}

static  unsigned long long MAX_FILE_SZ = GetMaxFileSz();

}

#endif // BMHCONSTANTS_H
