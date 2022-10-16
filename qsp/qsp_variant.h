#pragma once
#include "qsp_string.h"
class qsp_variant
{
public:
    union
    {
        qsp_string Str;
        int Num;
    } Val;
    char Type;
};

