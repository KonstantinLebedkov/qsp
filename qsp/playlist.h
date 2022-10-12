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
#include "variant.h"

    #define QSP_PLVOLUMEDELIM QSP_FMT("*")
    #define QSP_MAXPLFILES 500

    extern QSPString qspPLFiles[QSP_MAXPLFILES];
    extern int qspPLFilesCount;

    /* External functions */
    void qspClearPlayList(QSP_BOOL);
    void qspPlayPLFiles();
    void qspRefreshPlayList();
    /* Statements */
    QSP_BOOL qspStatementPlayFile(QSPVariant *args, QSP_TINYINT count, QSPString *jumpTo, QSP_TINYINT extArg);
    QSP_BOOL qspStatementCloseFile(QSPVariant *args, QSP_TINYINT count, QSPString *jumpTo, QSP_TINYINT extArg);