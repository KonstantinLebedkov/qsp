#pragma once
#include "qsp_LineOfCode.h"
class qsp_LinesOfCode
    // vector with strings of code. and methods for work with them
{
public:
    std::vector<qsp_LineOfCode*> Lines; //strings of code.
    qsp_LinesOfCode& operator=(const qsp_LinesOfCode& src);// copying constructor
    ~qsp_LinesOfCode() { for (auto it : Lines) it->~qsp_LineOfCode(); };//destructor
    bool ExecuteBlock();
    bool ExecuteCode();
};

