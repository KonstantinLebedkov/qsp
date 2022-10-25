#pragma once

#include "qsp_locActs.h"

class qsp_location
{
    String Name;
    String Desc;
    qsp_LinesOfCode OnVisitLines;
    int OnVisitLinesCount;
    qsp_locActs Actions;
};

