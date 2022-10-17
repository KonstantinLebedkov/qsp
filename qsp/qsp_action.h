#pragma once

#include <vector>
#include "qsp_variant.h"
#include "qsp_LinesOfCode.h"

class qsp_action
{
public:
    qsp_string* Desc; //name, description
    qsp_string* Image; //what is image? img file? something else?
    qsp_LinesOfCode* OnPressLines;//code for action
    int Location; //location. for handling errors
    int ActIndex; //index of action. for handling errors
    qsp_action(std::vector<qsp_variant*> args, qsp_LinesOfCode* code, int start, int end);
    ~qsp_action() { Image->~qsp_string(); Desc->~qsp_string(); OnPressLines->~qsp_LinesOfCode(); };
};

