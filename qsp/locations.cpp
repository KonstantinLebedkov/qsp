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

#include "locations.h"
#include "common.h"
#include "errors.h"
#include "game.h"
#include "statements.h"
#include "text.h"
#include "variables.h"

QSPLocation *qspLocs = 0; /*MARK: redesigned as vector qsp_world.Locs*/
QSPLocName *qspLocsNames = 0; /*MARK: redesigned as vector qsp_world.LocsNames*/
int qspLocsCount = 0; /*MARK not need anymore, there is size of vector - qsp_world.Locs.size()*/
int qspCurLoc = -1; /*MARK: redesigned as qsp_world.CurLoc*/
int qspRefreshCount = 0; /*MARK: redesigned as qsp_world.RefreshCount*/
int qspFullRefreshCount = 0; /*MARK: redesigned as qsp_world.FullRefreshCount*/

INLINE int qspLocsCompare(const void *, const void *);
INLINE int qspLocStringCompare(const void *, const void *);
INLINE void qspExecLocByIndex(int locInd, QSP_BOOL isChangeDesc);

INLINE int qspLocsCompare(const void *locName1, const void *locName2)
{
    return qspStrsComp(((QSPLocName *)locName1)->Name, ((QSPLocName *)locName2)->Name);
}

INLINE int qspLocStringCompare(const void *name, const void *compareTo)
{
    return qspStrsComp(*(QSPString *)name, ((QSPLocName *)compareTo)->Name);
}

void qspCreateWorld(int start, int locsCount)
{
    int i, j;
    for (i = start; i < qspLocsCount; ++i)
    {
        qspFreeString(qspLocsNames[i].Name);
        qspFreeString(qspLocs[i].Name);
        qspFreeString(qspLocs[i].Desc);
        qspFreePrepLines(qspLocs[i].OnVisitLines, qspLocs[i].OnVisitLinesCount);
        for (j = 0; j < QSP_MAXACTIONS; ++j)
        {
            if (qspLocs[i].Actions[j].Desc.Str)
            {
                qspFreeString(qspLocs[i].Actions[j].Image);
                qspFreeString(qspLocs[i].Actions[j].Desc);
                qspFreePrepLines(qspLocs[i].Actions[j].OnPressLines, qspLocs[i].Actions[j].OnPressLinesCount);
            }
        }
    }
    if (qspLocsCount != locsCount)
    {
        qspLocsCount = locsCount;
        qspLocs = (QSPLocation *)realloc(qspLocs, qspLocsCount * sizeof(QSPLocation));
        qspLocsNames = (QSPLocName *)realloc(qspLocsNames, qspLocsCount * sizeof(QSPLocName));
    }
    for (i = start; i < qspLocsCount; ++i)
    {
        qspLocsNames[i].Name = qspNullString;
        for (j = 0; j < QSP_MAXACTIONS; ++j)
            qspLocs[i].Actions[j].Desc = qspNullString;
    }
}

void qspPrepareLocs()
{
    int i;
    for (i = 0; i < qspLocsCount; ++i)
    {
        qspLocsNames[i].Index = i;
        qspUpdateText(&qspLocsNames[i].Name, qspLocs[i].Name);
        qspUpperStr(&qspLocsNames[i].Name);
    }
    qsort(qspLocsNames, qspLocsCount, sizeof(QSPLocName), qspLocsCompare);
}

int qspLocIndex(QSPString name)
{
    QSPLocName *loc;
    if (!qspLocsCount) return -1;
    name = qspDelSpc(name);
    if (qspIsEmpty(name)) return -1;
    name = qspGetNewText(name);
    qspUpperStr(&name);
    loc = (QSPLocName *)bsearch(&name, qspLocsNames, qspLocsCount, sizeof(QSPLocName), qspLocStringCompare);
    qspFreeString(name);
    if (loc) return loc->Index;
    return -1;
}

INLINE void qspExecLocByIndex(int locInd, QSP_BOOL isChangeDesc)
{
    QSPVariant actionArgs[2];
    qsp_string str;
    QSPLineOfCode *code;
    int i, count, oldLoc, oldActIndex, oldLine, oldRefreshCount = qspRefreshCount;
    QSPLocation *loc = qspLocs + locInd;
    /* remember a previous state to restore it after internal calls */
    oldLoc = qspRealCurLoc;
    oldActIndex = qspRealActIndex;
    oldLine = qspRealLine;
    /* switch the current state */
    qspRealCurLoc = locInd;
    qspRealActIndex = -1;
    qspRealLine = 0;
    str = qspFormatText(loc->Desc, QSP_FALSE);
    if (qspRefreshCount != oldRefreshCount || qspErrorNum)
    {
        qspRealLine = oldLine;
        qspRealActIndex = oldActIndex;
        qspRealCurLoc = oldLoc;
        return;
    }
    if (isChangeDesc)
    {
        qspCurDesc.clear();//was: qspFreeString(qspCurDesc);
        qspCurDesc = str;
        qspIsMainDescChanged = QSP_TRUE;
    }
    else
    {
        if (!qspIsEmpty(str))
        {
            qspCurDesc.append(str);//was: qspAddText(&qspCurDesc, str, QSP_FALSE);
            qspIsMainDescChanged = QSP_TRUE;
        }
        str.clear();//was: qspFreeString(str);
    }
    for (i = 0; i < QSP_MAXACTIONS; ++i)
    {
        str = loc->Actions[i].Desc;
        if (qspIsEmpty(str)) break;
        str = qspFormatText(str, QSP_FALSE);
        if (qspRefreshCount != oldRefreshCount || qspErrorNum)
        {
            qspRealLine = oldLine;
            qspRealActIndex = oldActIndex;
            qspRealCurLoc = oldLoc;
            return;
        }
        qspRealActIndex = i;
        actionArgs[0].Type = QSP_TYPE_STRING;
        QSP_STR(actionArgs[0]) = str;
        str = loc->Actions[i].Image;
        if (!qspIsEmpty(str))
        {
            actionArgs[1].Type = QSP_TYPE_STRING;
            QSP_STR(actionArgs[1]) = str;
            count = 2;
        }
        else
            count = 1;
        qspAddAction(actionArgs, count, loc->Actions[i].OnPressLines, 0, loc->Actions[i].OnPressLinesCount);
        qspFreeString(QSP_STR(actionArgs[0]));
        if (qspErrorNum)
        {
            qspRealLine = oldLine;
            qspRealActIndex = oldActIndex;
            qspRealCurLoc = oldLoc;
            return;
        }
    }
    qspRealActIndex = -1;
    if (locInd < qspLocsCount - qspCurIncLocsCount)
        qspExecCode(loc->OnVisitLines, 0, loc->OnVisitLinesCount, 1, 0);
    else
    {
        count = loc->OnVisitLinesCount;
        qspCopyPrepLines(&code, loc->OnVisitLines, 0, count);
        qspExecCode(code, 0, count, 1, 0);
        qspFreePrepLines(code, count);
    }
    /* restore the old state */
    qspRealLine = oldLine;
    qspRealActIndex = oldActIndex;
    qspRealCurLoc = oldLoc;
}

void qspExecLocByNameWithArgs(QSPString name, QSPVariant *args, QSP_TINYINT count, QSPVariant *res)
{
    QSPVar *varArgs, *varRes;
    int oldRefreshCount, locInd = qspLocIndex(name);
    if (locInd < 0)
    {
        qspSetError(QSP_ERR_LOCNOTFOUND);
        return;
    }
    if (!(varArgs = qspVarReference(QSP_STATIC_STR(QSP_VARARGS), QSP_TRUE))) return;
    if (!(varRes = qspVarReference(QSP_STATIC_STR(QSP_VARRES), QSP_TRUE))) return;
    qspAllocateSavedVarsGroupWithArgs(varArgs, varRes);
    qspSetArgs(varArgs, args, count);
    oldRefreshCount = qspRefreshCount;
    qspExecLocByIndex(locInd, QSP_FALSE);
    if (qspRefreshCount != oldRefreshCount || qspErrorNum)
    {
        qspReleaseSavedVarsGroup(QSP_TRUE);
        return;
    }
    if (res)
    {
        if (!(varRes = qspVarReference(QSP_STATIC_STR(QSP_VARRES), QSP_FALSE)))
        {
            qspReleaseSavedVarsGroup(QSP_TRUE);
            return;
        }
        qspApplyResult(varRes, res);
    }
    qspReleaseSavedVarsGroup(QSP_FALSE);
}

void qspExecLocByVarNameWithArgs(QSPString name, QSPVariant *args, QSP_TINYINT count)
{
    QSPVar *var;
    QSPString locName;
    int ind = 0, oldRefreshCount = qspRefreshCount;
    /* We execute all locations specified in the array */
    while (1)
    {
        if (!(var = qspVarReference(name, QSP_FALSE))) break;
        if (ind >= var->ValsCount) break;
        if (!QSP_ISSTR(var->Values[ind].Type)) break;
        locName = QSP_STR(var->Values[ind]);
        if (!(locName.Str && qspIsAnyString(locName))) break;
        qspExecLocByNameWithArgs(locName, args, count, 0);
        if (qspRefreshCount != oldRefreshCount || qspErrorNum) break;
        ++ind;
    }
}

void qspRefreshCurLoc(QSP_BOOL isChangeDesc, QSPVariant *args, QSP_TINYINT count)
{
    QSPVar *varArgs;
    int oldRefreshCount;
    if (qspCurLoc < 0) return;
    qspRestoreGlobalVars(); /* clean all local variables */
    if (qspErrorNum) return;
    /* We assign global ARGS here */
    if (!(varArgs = qspVarReference(QSP_STATIC_STR(QSP_VARARGS), QSP_TRUE))) return;
    qspEmptyVar(varArgs);
    qspSetArgs(varArgs, args, count);
    qspClearActions(QSP_FALSE);
    ++qspRefreshCount;
    if (isChangeDesc) ++qspFullRefreshCount;
    qspAllocateSavedVarsGroup();
    oldRefreshCount = qspRefreshCount;
    qspExecLocByIndex(qspCurLoc, isChangeDesc);
    if (qspRefreshCount != oldRefreshCount || qspErrorNum)
    {
        qspReleaseSavedVarsGroup(QSP_TRUE);
        return;
    }
    qspReleaseSavedVarsGroup(QSP_FALSE);
    if (qspErrorNum) return;
    qspExecLocByVarNameWithArgs(QSP_STATIC_STR(QSP_FMT("ONNEWLOC")), args, count);
}
