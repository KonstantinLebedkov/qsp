#pragma once
#include "qsp_CachedArg.h"
#include <vector>
class qsp_CachedStat
{
public:
    char Stat; //statement, type of it?
    int ParamPos;//position of parameter. //TODO: may be it will be better use iterators?
    int EndPos;//position of parameter. //TODO: may be it will be better use iterators?
    std::vector<qsp_CachedArg*> Args; //arguments of statement?
    int ErrorCode;//code of error, if called?
    qsp_CachedStat& operator=(const qsp_CachedStat& src);//copying constructor
    ~qsp_CachedStat();//destructor
};

