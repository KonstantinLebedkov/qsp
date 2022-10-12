/* Copyright (C) 2001-2020 Valeriy Argunov (byte AT qsp DOT org) */
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "../../declarations.h"

#ifdef _DEFAULT_BINDING

#include "../../actions.h"
#include "../../callbacks.h"
#include "../../coding.h"
#include "../../common.h"
#include "../../errors.h"
#include "../../game.h"
#include "../../locations.h"
#include "../../mathops.h"
#include "../../menu.h"
#include "../../objects.h"
#include "../../statements.h"
#include "../../text.h"
#include "../../time.h"
#include "../../variables.h"
#include "../../variant.h"

/* ------------------------------------------------------------ */
/* Debugger */

/* Enable the debugger */
void QSPEnableDebugMode(QSP_BOOL isDebug)
{
    qspIsDebug = isDebug;
}
/* Get current execution state */
void QSPGetCurStateData(QSPString *loc, int *actIndex, int *line)
{
    *loc = (qspRealCurLoc >= 0 && qspRealCurLoc < qspLocsCount ? qspLocs[qspRealCurLoc].Name : qspNullString);
    *actIndex = qspRealActIndex;
    *line = qspRealLine;
}
/* ------------------------------------------------------------ */
/* Version details */

/* Get version of the libqsp */
QSPString QSPGetVersion()
{
    return QSP_STATIC_STR(QSP_VER);
}
/* Get build datetime of the libqsp */
QSPString QSPGetCompiledDateTime()
{
    return QSP_STATIC_STR(QSP_FMT(__DATE__) QSP_FMT(", ") QSP_FMT(__TIME__));
}
/* ------------------------------------------------------------ */
/* Get number of the full location updates */
int QSPGetFullRefreshCount()
{
    return qspFullRefreshCount;
}
/* ------------------------------------------------------------ */
/* Main description */

/* Get text of the main description */
QSPString QSPGetMainDesc()
{
    return qspCurDesc;
}
/* Check whether the text has been updated */
QSP_BOOL QSPIsMainDescChanged()
{
    return qspIsMainDescChanged;
}
/* ------------------------------------------------------------ */
/* Additional description */

/* Get text of the additional description */
QSPString QSPGetVarsDesc()
{
    return qspCurVars;
}
/* Check whether the text has been updated */
QSP_BOOL QSPIsVarsDescChanged()
{
    return qspIsVarsDescChanged;
}
/* ------------------------------------------------------------ */
/* Synchronize the value of the text input control */
void QSPSetInputStrText(QSPString val)
{
    qspUpdateText(&qspCurInput, val);
}
/* ------------------------------------------------------------ */
/* Actions */

/* Get current actions */
int QSPGetActions(QSPListItem *items, int itemsBufSize)
{
    int i;
    for (i = 0; i < qspCurActionsCount && i < itemsBufSize; ++i)
    {
        items[i].Name = qspCurActions[i].Desc;
        items[i].Image = qspCurActions[i].Image;
    }
    return qspCurActionsCount;
}
/* Set index of a selected action */
QSP_BOOL QSPSetSelActionIndex(int ind, QSP_BOOL isRefresh)
{
    if (ind >= 0 && ind < qspCurActionsCount && ind != qspCurSelAction)
    {
        if (qspIsDisableCodeExec) return QSP_FALSE;
        qspPrepareExecution();
        qspCurSelAction = ind;
        qspExecLocByVarNameWithArgs(QSP_STATIC_STR(QSP_FMT("ONACTSEL")), 0, 0);
        if (qspErrorNum) return QSP_FALSE;
        if (isRefresh) qspCallRefreshInt(QSP_FALSE);
    }
    return QSP_TRUE;
}
/* Execute the selected action */
QSP_BOOL QSPExecuteSelActionCode(QSP_BOOL isRefresh)
{
    if (qspCurSelAction >= 0)
    {
        if (qspIsDisableCodeExec) return QSP_FALSE;
        qspPrepareExecution();
        qspExecAction(qspCurSelAction);
        if (qspErrorNum) return QSP_FALSE;
        if (isRefresh) qspCallRefreshInt(QSP_FALSE);
    }
    return QSP_TRUE;
}
/* Get index of the selected action */
int QSPGetSelActionIndex()
{
    return qspCurSelAction;
}
/* Check whether the actions have been updated */
QSP_BOOL QSPIsActionsChanged()
{
    return qspIsActionsChanged;
}
/* ------------------------------------------------------------ */
/* Objects */

/* Get current objects */
int QSPGetObjects(QSPListItem *items, int itemsBufSize)
{
    int i;
    for (i = 0; i < qspCurObjectsCount && i < itemsBufSize; ++i)
    {
        items[i].Name = qspCurObjects[i].Desc;
        items[i].Image = qspCurObjects[i].Image;
    }
    return qspCurObjectsCount;
}
/* Set index of a selected object */
QSP_BOOL QSPSetSelObjectIndex(int ind, QSP_BOOL isRefresh)
{
    if (ind >= 0 && ind < qspCurObjectsCount && ind != qspCurSelObject)
    {
        if (qspIsDisableCodeExec) return QSP_FALSE;
        qspPrepareExecution();
        qspCurSelObject = ind;
        qspExecLocByVarNameWithArgs(QSP_STATIC_STR(QSP_FMT("ONOBJSEL")), 0, 0);
        if (qspErrorNum) return QSP_FALSE;
        if (isRefresh) qspCallRefreshInt(QSP_FALSE);
    }
    return QSP_TRUE;
}
/* Get index of the selected object */
int QSPGetSelObjectIndex()
{
    return qspCurSelObject;
}
/* Check whether the objects have been updated */
QSP_BOOL QSPIsObjectsChanged()
{
    return qspIsObjectsChanged;
}
/* ------------------------------------------------------------ */
/* Synchronize visibility of a region of the UI */
void QSPShowWindow(int type, QSP_BOOL isShow)
{
    switch (type)
    {
    case QSP_WIN_ACTS:
        qspCurIsShowActs = isShow;
        break;
    case QSP_WIN_OBJS:
        qspCurIsShowObjs = isShow;
        break;
    case QSP_WIN_VARS:
        qspCurIsShowVars = isShow;
        break;
    case QSP_WIN_INPUT:
        qspCurIsShowInput = isShow;
        break;
    }
}
/* ------------------------------------------------------------ */
/* Variables */

/* Get number of items in an array */
QSP_BOOL QSPGetVarValuesCount(QSPString name, int *count)
{
    QSPVar *var;
    var = qspVarReference(name, QSP_FALSE);
    if (!var)
    {
        *count = 0;
        return QSP_FALSE;
    }
    *count = var->ValsCount;
    return QSP_TRUE;
}
/* Get values of the specified array item */
QSP_BOOL QSPGetVarValues(QSPString name, int ind, int *numVal, QSPString *strVal)
{
    QSPVar *var;
    var = qspVarReference(name, QSP_FALSE);
    if (!var || ind < 0 || ind >= var->ValsCount)
    {
        *strVal = qspNullString;
        *numVal = 0;
        return QSP_FALSE;
    }
    if (QSP_ISSTR(var->Values[ind].Type))
    {
        *strVal = QSP_STR(var->Values[ind]);
        *numVal = 0;
    }
    else
    {
        *strVal = qspNullString;
        *numVal = QSP_NUM(var->Values[ind]);
    }
    return QSP_TRUE;
}
/* Get max number of variables */
int QSPGetMaxVarsCount()
{
    return QSP_VARSCOUNT;
}
/* Get name of a variable by index */
QSP_BOOL QSPGetVarNameByIndex(int index, QSPString *name)
{
    if (index < 0 || index >= QSP_VARSCOUNT || qspIsEmpty(qspVars[index].Name))
    {
        *name = qspNullString;
        return QSP_FALSE;
    }
    *name = qspVars[index].Name;
    return QSP_TRUE;
}
/* ------------------------------------------------------------ */
/* Code execution */

/* Execute a line of code */
QSP_BOOL QSPExecString(QSPString s, QSP_BOOL isRefresh)
{
    if (qspIsDisableCodeExec) return QSP_FALSE;
    qspPrepareExecution();
    qspExecStringAsCodeWithArgs(s, 0, 0, 1, 0);
    if (qspErrorNum) return QSP_FALSE;
    if (isRefresh) qspCallRefreshInt(QSP_FALSE);
    return QSP_TRUE;
}
/* Execute code of the specified location */
QSP_BOOL QSPExecLocationCode(QSPString name, QSP_BOOL isRefresh)
{
    if (qspIsDisableCodeExec) return QSP_FALSE;
    qspPrepareExecution();
    qspExecLocByNameWithArgs(name, 0, 0, 0);
    if (qspErrorNum) return QSP_FALSE;
    if (isRefresh) qspCallRefreshInt(QSP_FALSE);
    return QSP_TRUE;
}
/* Execute code of the special "COUNTER" location */
QSP_BOOL QSPExecCounter(QSP_BOOL isRefresh)
{
    if (!qspIsInCallBack)
    {
        qspPrepareExecution();
        qspExecLocByVarNameWithArgs(QSP_STATIC_STR(QSP_FMT("COUNTER")), 0, 0);
        if (qspErrorNum) return QSP_FALSE;
        if (isRefresh) qspCallRefreshInt(QSP_FALSE);
    }
    return QSP_TRUE;
}
/* Execute code of the special "USERCOM" location */
QSP_BOOL QSPExecUserInput(QSP_BOOL isRefresh)
{
    if (qspIsDisableCodeExec) return QSP_FALSE;
    qspPrepareExecution();
    qspExecLocByVarNameWithArgs(QSP_STATIC_STR(QSP_FMT("USERCOM")), 0, 0);
    if (qspErrorNum) return QSP_FALSE;
    if (isRefresh) qspCallRefreshInt(QSP_FALSE);
    return QSP_TRUE;
}
/* ------------------------------------------------------------ */
/* Errors */

/* Get details of a last error */
void QSPGetLastErrorData(int *errorNum, QSPString *errorLoc, int *errorActIndex, int *errorLine)
{
    *errorNum = qspErrorNum;
    *errorLoc = (qspErrorLoc >= 0 && qspErrorLoc < qspLocsCount ? qspLocs[qspErrorLoc].Name : qspNullString);
    *errorActIndex = qspErrorActIndex;
    *errorLine = qspErrorLine;
}
/* Get error description by code */
QSPString QSPGetErrorDesc(int errorNum)
{
    return qspGetErrorDesc(errorNum);
}
/* ------------------------------------------------------------ */
/* Game controls */

/* Load game from data */
QSP_BOOL QSPLoadGameWorldFromData(const void *data, int dataSize, QSP_BOOL isNewGame)
{
    /* we don't execute any game code */
    return qspOpenGame((void *)data, dataSize, isNewGame);
}
/* Save game state to a buffer */
QSP_BOOL QSPSaveGameAsData(void *buf, int *bufSize, QSP_BOOL isRefresh)
{
    if (qspIsDisableCodeExec) return QSP_FALSE;
    qspPrepareExecution();
    if (!qspSaveGameStatus(buf, bufSize)) return QSP_FALSE;
    if (isRefresh) qspCallRefreshInt(QSP_FALSE);
    return QSP_TRUE;
}
/* Load game state from data */
QSP_BOOL QSPOpenSavedGameFromData(const void *data, int dataSize, QSP_BOOL isRefresh)
{
    if (qspIsDisableCodeExec) return QSP_FALSE;
    qspPrepareExecution();
    if (!qspOpenGameStatus((void *)data, dataSize)) return QSP_FALSE;
    if (isRefresh) qspCallRefreshInt(QSP_FALSE);
    return QSP_TRUE;
}
/* Restart current game */
QSP_BOOL QSPRestartGame(QSP_BOOL isRefresh)
{
    if (qspIsDisableCodeExec) return QSP_FALSE;
    qspPrepareExecution();
    qspNewGame(QSP_TRUE);
    if (qspErrorNum) return QSP_FALSE;
    if (isRefresh) qspCallRefreshInt(QSP_FALSE);
    return QSP_TRUE;
}
/* ------------------------------------------------------------ */
/* Configure callbacks */
void QSPSetCallBack(int type, QSP_CALLBACK func)
{
    qspSetCallBack(type, func);
}
/* ------------------------------------------------------------ */
/* Initialization of the engine */
void QSPInit()
{
    #ifdef _DEBUG
        mwInit();
    #endif
    qspNullString = qspStringFromPair(0, 0);
    qspIsDebug = QSP_FALSE;
    qspRefreshCount = qspFullRefreshCount = 0;
    qspQstCRC = 0;
    qspRealCurLoc = -1;
    qspRealActIndex = -1;
    qspRealLine = 0;
    qspMSCount = 0;
    qspLocs = 0;
    qspLocsNames = 0;
    qspLocsCount = 0;
    qspCurLoc = -1;
    qspTimerInterval = 0;
    qspCurIsShowObjs = qspCurIsShowActs = qspCurIsShowVars = qspCurIsShowInput = QSP_TRUE;
    setlocale(LC_ALL, QSP_LOCALE);
    qspSetSeed(0);
    qspPrepareExecution();
    qspMemClear(QSP_TRUE);
    qspInitCallBacks();
    qspInitStats();
    qspInitMath();
}
/* Deallocate all the resources */
void QSPDeInit()
{
    qspMemClear(QSP_FALSE);
    qspCreateWorld(0, 0);
    #ifdef _DEBUG
        mwTerm();
    #endif
}

#endif
