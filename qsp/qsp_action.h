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
    qsp_action() { Image = new qsp_string(); Desc = new qsp_string(); };
    //TODO: requred constructor with predefined fields
    ~qsp_action() { Image->~qsp_string(); Desc->~qsp_string(); };
};

