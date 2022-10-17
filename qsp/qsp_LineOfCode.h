#pragma once
#include "qsp_string.h"
#include "qsp_CachedStat.h"
class qsp_LineOfCode
{
public:
    qsp_string Str;
    int LineNum;
    bool IsMultiline;
    qsp_string Label;
    qsp_CachedStat* Stats;
    int StatsCount;
};

