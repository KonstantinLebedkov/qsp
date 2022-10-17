#include "qsp_CachedStat.h"

qsp_CachedStat& qsp_CachedStat::operator=(const qsp_CachedStat& src)
{
    this->Stat = src.Stat;
    this->ParamPos = src.ParamPos;
    this->EndPos = src.EndPos;
    this->ErrorCode = src.ErrorCode;
    if (!src.Args.empty())
    {
        for (auto it : src.Args)
        {
            Args.push_back(new qsp_CachedArg(*it));
        }
    }
    return *this;
}

qsp_CachedStat::~qsp_CachedStat()
{
    for (qsp_CachedArg* Arg : Args)
    {
        Arg->~qsp_CachedArg();
    }
}
