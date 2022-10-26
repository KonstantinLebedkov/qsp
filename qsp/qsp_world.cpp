#include "qsp_world.h"
#include "qsp_variables.h"
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
{//TODO: ������ ��������, ����-�����, ���� �������� ������������� ����, ��! �� �� �� �����, � ��� ������������ ��������� ������. ������� �� ��������� ������� ���������.
    //TODO: �� ������ ��������� ����� �����, ���� �������� �� ������� ������� �����. ��� ����������/����������� ��������� ����.
    int i, j;
    Locs.ClearTail(start);
    LocsNames.ClearTail(start);
}

void qsp_world::PrepareLocs()
{
    int i;
    LocsNames.Clear();
    for (auto it : Locs) LocsNames.push_back(qsp_locName(it));
    //����� ���� ��������� ����������� �� �����.
    LocsNames.Sort();
}

void qsp_world::RefreshCurLoc(bool isChangeDesc, qsp_variants args, char count)
{
    qsp_var * varArgs;
    int oldRefreshCount;
    if (qspCurLoc < 0) return;
    VARS.RestoreGlobalVars(); /* clean all local variables */
    if (ErrorNum) return;
    /* We assign global ARGS here */
    if (!(varArgs = VARS.VarReference(String(QSP_VARARGS)))) return;
    varArgs->EmptyVar();//qspEmptyVar(varArgs);
    varArgs->SetArgs(args);//qspSetArgs(varArgs, args, count);
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