#pragma once
#include "qsp_string.h"

class qsp_action
{
public:
    qsp_string* Image;
    qsp_string* Desc;
    QSPLineOfCode* OnPressLines;
    int OnPressLinesCount;
    int Location;
    int ActIndex;
};

