#include "qsp_game.h"

void qsp_game::ClearIncludes(bool isFirst)
{
    int i, count;
    if (!isFirst)
    {//TODO: первое - уходим от массива фиксированной длины. и потому размер массива нам хратить без надобности, ибо у вестора есть сайз.
        //TODO: второе. вектор стрингов надо таки определить отдельным классом, коему вписать метод очистки.
        CurIncFiles.ClearStrings();
        if (CurIncLocsCount)
        {
            count = World.LocsCount() - CurIncLocsCount;
            //по разнице между счетчиком локаций в гейме и в мире(фасаде локаций) создаем мир.... ну как-то дико.
            World.CreateWorld(count, count);
            //а потом оные локации готовим.
            qspPrepareLocs();
            //и в итоге принудительно пишем что ноль файлов, ноль локаций.
        }
    }
    CurIncLocsCount = 0;
}
