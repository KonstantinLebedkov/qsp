#include "qsp_world.h"

qsp_world& qsp_world::WorldHandler()
{
    static qsp_world theHandler;
    return theHandler;
}

int qsp_world::LocsCount()
{
    return Locs.size();
}

void qsp_world::CreateWorld(int start, int locsCount)
{//TODO: второй аргумент, локс-каунт, пока оставлен совместимости ради, но! он же не нужен, у нас динамическая структура теперь. удалить на очережном проходе редизайна.
    //TODO: во втором аргументе будет смысл, если вырезать из массива локаций куски. для отключения/подключения библиотек игры.
    int i, j;
    Locs.ClearTail(start);
    LocsNames.ClearTail(start);
}

void qsp_world::PrepareLocs()
{
    int i;
    LocsNames.Clear();
    for (auto it : Locs) LocsNames.push_back(qsp_locName(it));
    //после чего указатели сортируются по имени.
    LocsNames.Sort();
}
