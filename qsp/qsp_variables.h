#pragma once
#include <array>
#include "qsp_varsGroup.h"

#define VARS qsp_variables::VarHandler()

#define QSP_VARSGROUPS 256

class qsp_variables
{
    qsp_variables() {};
    qsp_variables(const qsp_variables& root) = delete;
    qsp_variables& operator=(const qsp_variables& root) = delete;
public:
    static qsp_variables& VarHandler();
	~qsp_variables();
    /* properties */
	std::array<qsp_varsGroup, QSP_VARSGROUPS> Vars; //array of vectors.
    //TODO: may be it will be better to use maps
    std::vector<qsp_varsGroup*> SavedVarGroups;
    /* methods */
	qsp_var* VarReference(String name);
    void RestoreGlobalVars();
};

