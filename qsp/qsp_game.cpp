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

void qsp_game::NewGame(bool isReset)
{
    if (!qspLocsCount)
    {
        qspSetError(QSP_ERR_GAMENOTLOADED);
        return;
    }
    qspCurLoc = 0;
    if (isReset)
    {
        qspSetSeed((unsigned int)QSP_TIME(0));
        qspTimerInterval = QSP_DEFTIMERINTERVAL;
        qspCurIsShowObjs = qspCurIsShowActs = qspCurIsShowVars = qspCurIsShowInput = QSP_TRUE;
        qspMemClear(QSP_FALSE);
        qspResetTime(0);
        qspCallShowWindow(QSP_WIN_ACTS, QSP_TRUE);
        qspCallShowWindow(QSP_WIN_OBJS, QSP_TRUE);
        qspCallShowWindow(QSP_WIN_VARS, QSP_TRUE);
        qspCallShowWindow(QSP_WIN_INPUT, QSP_TRUE);
        qspCallSetInputStrText(qspNullString);
        qspCallShowPicture(qspNullString);
        qspCallCloseFile(qspNullString);
        qspCallSetTimer(QSP_DEFTIMERINTERVAL);
    }
    qspRefreshCurLoc(QSP_TRUE, 0, 0);
}
