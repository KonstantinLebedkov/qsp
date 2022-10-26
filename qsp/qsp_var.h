#pragma once

#include "qsp_variants.h"

class qsp_var
{
    String Name;
    qsp_variants Values;
    int ValsCount;
    QSPVarIndex* Indices;
    int IndsCount;
    int IndsBufSize;
};

