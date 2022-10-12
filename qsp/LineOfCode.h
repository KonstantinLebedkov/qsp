#pragma once
#include <string>
#include "CachedStat.h"

class QSPLineOfCode
{ //TODO: лайн оф код вообще вынести в отдельный, нормальный, класс!!
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