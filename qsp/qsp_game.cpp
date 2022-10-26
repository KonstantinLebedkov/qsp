#include "qsp_game.h"

qsp_game& qsp_game::GameHandler()
{
    static qsp_game theHandler;
    return theHandler;
}

void qsp_game::ClearIncludes(bool isFirst)
{
    int i, count;
    if (!isFirst)
    {
        CurIncFiles.ClearStrings();
        if (CurIncLocsCount)
        {
            count = World.LocsCount() - CurIncLocsCount;
            //по разнице между счетчиком локаций в гейме и в мире(фасаде локаций) создаем мир.... ну как-то дико.
            World.CreateWorld(count, count); //удаляем локи номером свыше count
            World.PrepareLocs(); //обновляем вектор имен локаций
            //и в итоге принудительно пишем что ноль файлов, ноль локаций.
        }
    }
    CurIncLocsCount = 0;
}
