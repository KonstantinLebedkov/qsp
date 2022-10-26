#include "qsp_var.h"
#include "qsp_errors.h"

void qsp_var::EmptyVar()
{
    int count;
    if (Values.size()) Values.ClearVariants();
    if (Indices.size()) Indices.clear();
    InitVarData(); //is it really required?! natively, its just full with zero's
}

void qsp_var::InitVarData()
{
    //TODO: it may be better to use default constructor with initalizing values for items in this vectors?
    //Values = { 0 };
    //Indices = 0;
}

void qsp_var::SetArgs(qsp_variants args)
{
    for (auto it : args) Values.push_back(new qsp_variant(*it));
}
