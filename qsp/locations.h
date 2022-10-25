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
#pragma once

#include "declarations.h"
#include "actions.h"
#include "codetools.h"
#include "variant.h"

    typedef struct
    {
        QSPString Image;
        QSPString Desc;
        QSPLineOfCode *OnPressLines;
        int OnPressLinesCount;
    } QSPLocAct;
    typedef struct
    {
        QSPString Name;
        QSPString Desc;
        QSPLineOfCode *OnVisitLines;
        int OnVisitLinesCount;
        QSPLocAct Actions[QSP_MAXACTIONS];
    } QSPLocation; /*MARK: redesigned as qsp_location*/
    typedef struct
    {
        int Index;
        QSPString Name;
    } QSPLocName; /*MARK: redesigned as qsp_locName*/

    extern QSPLocation *qspLocs;
    extern QSPLocName *qspLocsNames;
    extern int qspLocsCount;
    extern int qspCurLoc;
    extern int qspRefreshCount;
    extern int qspFullRefreshCount;

    /* External functions */
    void qspCreateWorld(int, int);
    void qspPrepareLocs();
    int qspLocIndex(QSPString name);
    void qspExecLocByNameWithArgs(QSPString name, QSPVariant *args, QSP_TINYINT count, QSPVariant *res);
    void qspExecLocByVarNameWithArgs(QSPString name, QSPVariant *args, QSP_TINYINT count);
    void qspRefreshCurLoc(QSP_BOOL isChangeDesc, QSPVariant *args, QSP_TINYINT count);