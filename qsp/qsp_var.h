#pragma once

#include "qsp_variants.h"
#include "qsp_varIndex.h"

class qsp_var
{
public:
    String Name;
    qsp_variants Values;
    //int ValsCount; not required, because vector "Values" have method size();
    std::vector<qsp_varIndex> Indices;
    int IndsCount;
    int IndsBufSize;
    void EmptyVar();
};

