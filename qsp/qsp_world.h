#pragma once

#include "qsp_location.h"
#include "qsp_locNames.h"

class qsp_world
{
    qsp_location Locs;
    qsp_locNames LocsNames;
    //int qspLocsCount;
    int CurLoc = -1;
    int RefreshCount = 0;
    int FullRefreshCount = 0;
};

