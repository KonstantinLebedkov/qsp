#pragma once
#include <vector>
#include "CachedArg.h"

class QSPCachedStat
{
    char Stat;
    int ParamPos;
    int EndPos;
    std::vector<QSPCachedArg*> Args; //TODO: ����� ������ ���� ������.
    //QSP_TINYINT ArgsCount; //� ��� ������ �� ���� ��� � ������ ���� ����
    int ErrorCode;
};