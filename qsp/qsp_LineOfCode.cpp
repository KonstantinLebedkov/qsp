#include "qsp_LineOfCode.h"


qsp_LineOfCode& qsp_LineOfCode::operator=(const qsp_LineOfCode& src)
{
    this->Str = src.Str;
    this->LineNum = src.LineNum;
    if (!src.Stats.empty())
    {
        for (auto it : src.Stats)
        {
            Stats.push_back(new qsp_CachedStat(*it));
        }
    }
    this->IsMultiline = src.IsMultiline;
    this->Label = src.Label;
    return *this;
};

qsp_LineOfCode::~qsp_LineOfCode()
{ 
    for (auto it : Stats) 
        it->~qsp_CachedStat();
};