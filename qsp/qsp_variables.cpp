#include "qsp_variables.h"
#include "qsp_errors.h"

qsp_variables& qsp_variables::VarHandler()
{
    static qsp_variables theHandler;
    return theHandler;
}

qsp_variables::~qsp_variables()
{
    for (auto it : Vars) 
        it.~qsp_varsGroup(); 
    while (SavedVarGroups.size())
    {
        (*SavedVarGroups.rbegin())->~qsp_varsGroup();
        SavedVarGroups.pop_back();
    }
}

qsp_var* qsp_variables::VarReference(String name)
{
    String::iterator pos;
    unsigned char bCode;
    if (name.empty())
    {
        SetError(QSP_ERR_INCORRECTNAME);
        return 0;
    }
    if (*name.begin() == QSP_STRCHAR[0]) name.SkipN(String(QSP_STRCHAR).length()); /* ignore type specification */
    //если первый символ - бакс, скипнуть его.
    if (name.empty() || name.IsDigit() || name.IsAnyInClass(QSP_CHAR_DELIM))
    {
        SetError(QSP_ERR_INCORRECTNAME);
        return 0;
    }
    bCode = 7;
    for (pos = name.begin(); pos < name.end(); ++pos)
        bCode = bCode * 31 + (unsigned char)*pos;
    for (auto it:Vars[bCode])
    {
        if (!(name.compare(it->Name))) return it;
    }
    if (Create) Vars[bCode].push_back(new qsp_var(name));
    return *Vars[bCode].rbegin();
}

void qsp_variables::RestoreGlobalVars()
{
    while (SavedVarGroups.size())
    {
        (*SavedVarGroups.rbegin())->~qsp_varsGroup();
        SavedVarGroups.pop_back();
    };
}
