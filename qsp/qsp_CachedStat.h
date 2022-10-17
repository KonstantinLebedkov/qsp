#pragma once
#include "qsp_CachedArg.h"
class qsp_CachedStat
{
public:
    char Stat;
    int ParamPos;
    int EndPos;
    qsp_CachedArg* Args;
    char ArgsCount;
    int ErrorCode;
};

