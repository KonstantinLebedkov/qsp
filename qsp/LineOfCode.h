#pragma once
#include <string>
#include "CachedStat.h"

class QSPLineOfCode
{ //TODO: ���� �� ��� ������ ������� � ���������, ����������, �����!!
    QSPString *Str;
    int LineNum;
    bool IsMultiline;
    QSPString Label;
    QSPCachedStat* Stats;
    int StatsCount;
    int StatsCount;
    int Stats;
    QSPString qspGetLineLabel(QSPString str);
    char qspGetStatCode(QSPString s, QSP_CHAR** pos);
public:
    QSPLineOfCode(QSPString str, int lineNum) {};
};