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
#include "codetools.h"
#include "variant.h"

    #define QSP_MAXACTIONS 50

    typedef struct
    {
        QSPString Image;
        QSPString Desc;
        QSPLineOfCode *OnPressLines;
        int OnPressLinesCount;
        int Location;
        int ActIndex;
    } QSPCurAct;

    extern QSPCurAct qspCurActions[QSP_MAXACTIONS];
    extern int qspCurActionsCount;
    extern int qspCurSelAction;
    extern QSP_BOOL qspIsActionsChanged;
    extern QSP_BOOL qspCurIsShowActs;

    /* External functions */
    void qspClearActions(QSP_BOOL);
    void qspAddAction(QSPVariant *args, QSP_TINYINT count, QSPLineOfCode *code, int start, int end);
    void qspExecAction(int);
    QSPString qspGetAllActionsAsCode();
    /* Statements */
    void qspStatementSinglelineAddAct(QSPLineOfCode *s, int statPos, int endPos);
    void qspStatementMultilineAddAct(QSPLineOfCode *s, int lineInd, int endLine);
    QSP_BOOL qspStatementDelAct(QSPVariant *args, QSP_TINYINT count, QSPString *jumpTo, QSP_TINYINT extArg);