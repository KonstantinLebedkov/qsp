#pragma once

#include <vector>
#include "qsp_variant.h"
#include "qsp_LineOfCode.h"

class qsp_action
{
public:
    qsp_string* Image;
    qsp_string* Desc;
    qsp_LineOfCode* OnPressLines;
    int OnPressLinesCount;
    int Location;
    int ActIndex;
    qsp_action(std::vector<qsp_variant*> args, qsp_LineOfCode* code, int start, int end);
    //TODO: constructor with predefined fields must be updated with: 1. QSPLineOfCode.
    ~qsp_action() { Image->~qsp_string(); Desc->~qsp_string(); };
};

