#pragma once

#include "qsp_variants.h"
#include "qsp_varIndex.h"

class qsp_var
{
public:
    String Name; //name of variable
    qsp_variants Values; //vector of variant values
    std::vector<qsp_varIndex> Indices; // vector of Indexes
    qsp_var(String name):Name(name){};
    ~qsp_var() { EmptyVar(); };
    int IndsBufSize;
    void EmptyVar();// clear "Values" and "Indices"
    void InitVarData();//not required? its empty now...
    void SetArgs(qsp_variants args); //make property "Values" the copy of given vector of qsp_variant (makes new objects and place pointers of it into vector)   
};

