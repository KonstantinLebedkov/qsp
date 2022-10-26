#pragma once

#include "qsp_string.h"
#include "qsp_LinesOfCode.h"

class qsp_locAct
{
public:
    String Image;
    String Desc;
    qsp_LinesOfCode OnPressLines;
    int OnPressLinesCount;
    ~qsp_locAct();
};

