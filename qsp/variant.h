/* Copyright (C) 2001-2020 Valeriy Argunov (byte AT qsp DOT org) */
/* Copyright (C) 2022 Konstantin Lebedkov*/
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#pragma once
#include "declarations.h"
#include "text.h"


#define QSP_STR(a) (a).Val.Str
#define QSP_NUM(a) (a).Val.Num
#define QSP_PSTR(a) (a)->Val.Str
#define QSP_PNUM(a) (a)->Val.Num

enum
{
    QSP_TYPE_NUMBER = 0,
    QSP_TYPE_STRING = 1,
    QSP_TYPE_CODE = 2,
    QSP_TYPE_TUPLE = 3,
    QSP_TYPE_VARREF = 4,
    QSP_TYPE_DEFINED_TYPES = 5, /* represents a number of defined values */
    QSP_TYPE_UNDEFINED = 64, /* not used for values, it has to be a string-based type */
};

#define QSP_ISDEF(a) ((a) != QSP_TYPE_UNDEFINED)
#define QSP_ISNUM(a) ((a) == QSP_TYPE_NUMBER)
#define QSP_ISSTR(a) ((a) > QSP_TYPE_NUMBER)
#define QSP_BASETYPE(a) ((a) > QSP_TYPE_NUMBER) /* QSP_TYPE_STRING | QSP_TYPE_NUMBER */

//TODO: искренне непонимаю: накуа здесь делать юнион, если стринг прокастовывается в инт (при условии что он способен быть прокастован). и тогда достаточно просто флага, можно ли кастовать. да и то - флаг не обязателен, ведь ожидая инта, можно ПОПРОБОВАТЬ прокастовать. и там есть соответствующие реакции же, нет? ну 21й век на дворе, есть же плюсы! давно было пора обновить всё! я понимаю там флоат/инт/чар/структурку под юнион пихать. но стринг+инт? ересь!
struct QSPVariant
{
    union
    {
        QSPString Str;
        int Num;
    } Val;
    QSP_TINYINT Type;
};

    /* External functions */
QSP_BOOL qspConvertVariantTo(QSPVariant *val, QSP_TINYINT type);
int qspAutoConvertCompare(QSPVariant *v1, QSPVariant *v2);
void qspUpdateVariantValue(QSPVariant *dest, QSPVariant *src);

INLINE void qspFreeVariants(QSPVariant *args, int count)
//TODO: здесь статик войд, убивающий массив переменных типа КУСП_Вариант из памяти. 
{
    while (--count >= 0)
        if (QSP_ISSTR(args[count].Type)) qspFreeString(QSP_STR(args[count]));
}

INLINE void qspInitVariant(QSPVariant *value, QSP_TINYINT type)
//TODO: здесь статик войд, набивающий КУСП_Вариант по пойнтеру нулем либо нуль-пойнтером в зависимости от указанного ему типа
{
    if (QSP_ISSTR(value->Type = type))
        QSP_PSTR(value) = qspNullString;
    else
        QSP_PNUM(value) = 0;
}

INLINE QSPVariant qspGetEmptyVariant(QSP_TINYINT type)
//TODO: статик войд, создающий пустой КУСП_Вариант, причем делающий это, цуко, в стеке!!! нет, я бы понял если бы размещал в куче. но! городить всё в стеке... ну ахтунг же!
{
    QSPVariant QVar;
    qspInitVariant(&QVar, type);
    return QVar;
}

INLINE void qspCopyToNewVariant(QSPVariant *dest, QSPVariant *src)
//TODO: копирует значение из одного КУСП_Варианта в другой, причем логика определения как копировать.. она ущербная. совсем ущербная. потому что выбирается не в зависимости от типа их значений, а в зависимости от совпадения типов их значений. нет, в отдельных случаях эта иезуитская зашифрованная схема выбора могла бы быть уместна - но ЗДЕСЬ?!
{
    if (QSP_ISSTR(dest->Type = src->Type))
        QSP_PSTR(dest) = qspGetNewText(QSP_PSTR(src));
    else
        QSP_PNUM(dest) = QSP_PNUM(src);
}

INLINE QSP_BOOL qspIsCanConvertToNum(QSPVariant *val)
{
    QSP_BOOL isValid;
    if (QSP_ISSTR(val->Type))
    {
        qspStrToNum(QSP_PSTR(val), &isValid);
        if (!isValid) return QSP_FALSE;
    }
    return QSP_TRUE;
}