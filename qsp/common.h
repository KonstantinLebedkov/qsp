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

    #define QSP_RANDMASK 0x7FFFFFFF

    extern QSP_BOOL qspIsDebug;
    //extern QSPString qspCurDesc;
    qsp_string qspCurDesc;
    extern QSPString qspCurVars;
    extern QSPString qspCurInput;
    extern QSPString qspViewPath;
    extern int qspTimerInterval;
    extern QSP_BOOL qspIsMainDescChanged;
    extern QSP_BOOL qspIsVarsDescChanged;
    extern QSP_BOOL qspCurIsShowVars;
    extern QSP_BOOL qspCurIsShowInput;

    /* External functions */
    void qspPrepareExecution();
    void qspMemClear(QSP_BOOL);
    void qspSetSeed(unsigned int);
    int qspRand();