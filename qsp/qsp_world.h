#pragma once

#include "qsp_locations.h"
#include "qsp_locNames.h"

class qsp_world
{
public:
    qsp_locations Locs;
    qsp_locNames LocsNames;
    int CurLoc = -1;
    int RefreshCount = 0;
    int FullRefreshCount = 0;
    /* External functions */
    void CreateWorld(int start, int locsCount);
    void PrepareLocs();
    int LocIndex(String name);
    void ExecLocByNameWithArgs(String name, QSPVariant* args, char count, QSPVariant* res);
    void ExecLocByVarNameWithArgs(String name, QSPVariant* args, char count);
    void RefreshCurLoc(bool isChangeDesc, QSPVariant* args, char count);
};

