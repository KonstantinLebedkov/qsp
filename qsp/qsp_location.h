#pragma once

#include "qsp_string.h"

class qsp_location
{
    String Name;
    String Desc;
    QSPLineOfCode* OnVisitLines;
    int OnVisitLinesCount;
    QSPLocAct Actions[QSP_MAXACTIONS];
};

