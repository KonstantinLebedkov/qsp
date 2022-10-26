#include "qsp_world.h"
#include "qsp_varsGroup.h"
#include "qsp_errors.h"

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

void qsp_world::RefreshCurLoc(bool isChangeDesc, qsp_variants args, char count)
{
    qsp_varsGroup varArgs;
    int oldRefreshCount;
    if (qspCurLoc < 0) return;
    qspRestoreGlobalVars(); /* clean all local variables */ //procedure from variables.h
    if (ErrorNum) return;
    /* We assign global ARGS here */
    if (!(varArgs = qspVarReference(QSP_STATIC_STR(QSP_VARARGS), true))) return;
    qspEmptyVar(varArgs);
    qspSetArgs(varArgs, args, count);
    qspClearActions(false);
    ++qspRefreshCount;
    if (isChangeDesc) ++qspFullRefreshCount;
    qspAllocateSavedVarsGroup();
    oldRefreshCount = qspRefreshCount;
    qspExecLocByIndex(qspCurLoc, isChangeDesc);
    if (qspRefreshCount != oldRefreshCount || ErrorNum)
    {
        qspReleaseSavedVarsGroup(true);
        return;
    }
    qspReleaseSavedVarsGroup(false);
    if (ErrorNum) return;
    qspExecLocByVarNameWithArgs(QSP_STATIC_STR(QSP_FMT("ONNEWLOC")), args, count);
}