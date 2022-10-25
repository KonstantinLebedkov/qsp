#pragma once

#include "qsp_locations.h"
#include "qsp_locNames.h"

#define World qsp_world::WorldHandler()

class qsp_world
{
    qsp_world() {};
    qsp_world(const qsp_world& root) = delete;
    qsp_world& operator=(const qsp_world& root) = delete;
public:
    static qsp_world& WorldHandler();
    qsp_locations Locs;
    qsp_locNames LocsNames;
    int CurLoc = -1;
    int RefreshCount = 0;
    int FullRefreshCount = 0;
    /* External functions */
    int LocsCount(); //return amount of Locs
    void CreateWorld(int start, int locsCount);
    void PrepareLocs();
    int LocIndex(String name);
    void ExecLocByNameWithArgs(String name, QSPVariant* args, char count, QSPVariant* res);
    void ExecLocByVarNameWithArgs(String name, QSPVariant* args, char count);
    void RefreshCurLoc(bool isChangeDesc, QSPVariant* args, char count);
};

