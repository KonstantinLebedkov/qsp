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
            //�� ������� ����� ��������� ������� � ����� � � ����(������ �������) ������� ���.... �� ���-�� ����.
            World.CreateWorld(count, count); //������� ���� ������� ����� count
            World.PrepareLocs(); //��������� ������ ���� �������
            //� � ����� ������������� ����� ��� ���� ������, ���� �������.
        }
    }
    CurIncLocsCount = 0;
}

void qsp_game::NewGame(bool isReset)
{
    if (!World.LocsCount())
    {
        SetError(QSP_ERR_GAMENOTLOADED);
        return;
    }
    World.CurLoc = 0;
    if (isReset)
    {
        qspSetSeed((unsigned int)QSP_TIME(0)); //procedure from common.h
        qspTimerInterval = QSP_DEFTIMERINTERVAL; //global var from common.h
        qspCurIsShowObjs = qspCurIsShowActs = qspCurIsShowVars = qspCurIsShowInput = QSP_TRUE; //globals
        qspMemClear(QSP_FALSE); //procedure from common.h
        qspResetTime(0); //procedure from time.h
        qspCallShowWindow(QSP_WIN_ACTS, QSP_TRUE); //procedure from callbacks.h
        qspCallShowWindow(QSP_WIN_OBJS, QSP_TRUE);//procedure from callbacks.h
        qspCallShowWindow(QSP_WIN_VARS, QSP_TRUE);//procedure from callbacks.h
        qspCallShowWindow(QSP_WIN_INPUT, QSP_TRUE);//procedure from callbacks.h
        qspCallSetInputStrText(qspNullString);//procedure from callbacks.h
        qspCallShowPicture(qspNullString);//procedure from callbacks.h
        qspCallCloseFile(qspNullString);//procedure from callbacks.h
        qspCallSetTimer(QSP_DEFTIMERINTERVAL);//procedure from callbacks.h
    }
    World.RefreshCurLoc(true, 0, 0); //procedure from locations.h
}
