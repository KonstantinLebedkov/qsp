#pragma once
#include "qsp_string.h"
#include "qsp_CachedStat.h"
class qsp_LineOfCode
{
public:
    qsp_string Str; //string of line?
    int LineNum; //number of line in code?
    bool IsMultiline;//is part of multistring?
    qsp_string Label; //label of string (if exists)
    std::vector<qsp_CachedStat*> Stats; //statements in the string
    //TODO: ma be vector of stat make as class?
    qsp_LineOfCode& operator=(const qsp_LineOfCode& src);//copying constructor
    ~qsp_LineOfCode();//destructor
    bool ExecuteMultilineCode(qsp_LineOfCode* s, int endLine, int codeOffset, qsp_string* jumpTo, int* lineInd, int* action);
};

