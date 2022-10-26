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
    qsp_locations Locs; //вектор пойнтеров на объекты локаций
    qsp_locNames LocsNames; //вектор объектов с указателями и именами локаций, сортированный по именам локаций.
    //TODO: у меня все равно вектора указателей. не вижу серьезной потребности в отдельном векторе с именами. хотя, на этапе добавления/удаления библиотек это може быть потребно.
    int CurLoc = -1; //текущая локация. мб. в пойнтер на локу её переделать?
    int RefreshCount = 0;
    int FullRefreshCount = 0;
    /* External functions */
    int LocsCount(); //return amount of Locs
    void CreateWorld(int start, int locsCount); //чистит локи сверх количества "старт"
    void PrepareLocs(); //чистит вектор лок-неймз, заполняет и сортирует.
    int LocIndex(String name);
    void ExecLocByNameWithArgs(String name, QSPVariant* args, char count, QSPVariant* res);
    void ExecLocByVarNameWithArgs(String name, QSPVariant* args, char count);
    void RefreshCurLoc(bool isChangeDesc, QSPVariant* args, char count);
};

