#pragma once
#include <vector>
#include "CachedArg.h"

class QSPCachedStat
{
    char Stat;
    int ParamPos;
    int EndPos;
    std::vector<QSPCachedArg*> Args; //TODO: здесь должен быть вектор.
    //QSP_TINYINT ArgsCount; //а это вообще не надо ибо у ветора есть сайз
    int ErrorCode;
};