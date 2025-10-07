/*
Заголовочный файл, описывающий
интерфейс для установки и проверки свойства корректности
структуры.
Обновлен  11.08.2023
*/

#ifndef IVALID_H
#define IVALID_H

class IValid
{
public :

    IValid() : valid(true) {}

    IValid(const IValid &) = delete;

    virtual bool IsValid() const
    {
        return valid;
    }

    void SetValid(bool f)
    {
        valid = f;
    }

private :

    bool valid;

};

#endif // IVALID_H
