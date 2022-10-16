#pragma once

#include <vector>
#include "qsp_variant.h"

class qsp_action
{
public:
    qsp_string* Image;
    qsp_string* Desc;
    QSPLineOfCode* OnPressLines;
    int OnPressLinesCount;
    int Location;
    int ActIndex;
    qsp_action(std::vector<qsp_variant*> args, QSPLineOfCode* code, int start, int end);
    //TODO: constructor with predefined fields must be updated with: 1. QSPLineOfCode.
    ~qsp_action() { Image->~qsp_string(); Desc->~qsp_string(); };
};

