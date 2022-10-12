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

    #define QSP_OPSLEVELS 2
    #define QSP_MAXOPSNAMES 100
    #define QSP_OPMAXARGS 20
    #define QSP_STACKSIZE 30
    #define QSP_MAXITEMS 200
    #define QSP_VALSDELIM QSP_FMT("\x1F")

    /* Helpers */
    #define QSP_TOBOOL(x) ((x) != 0) /* converts a number to a QSP boolean value */
    #define QSP_ISTRUE(x) ((x) != 0) /* checks whether a QSP numeric value represents boolean 'true' */

    typedef void (*QSP_FUNCTION)(QSPVariant *, QSP_TINYINT, QSPVariant *);

    typedef struct
    {
        QSP_TINYINT Code;
        QSPString Name;
    } QSPMathOpName;

    typedef struct
    {
        QSP_TINYINT Priority;
        QSP_TINYINT ResType;
        QSP_TINYINT MinArgsCount;
        QSP_TINYINT MaxArgsCount;
        QSP_TINYINT ArgsTypes[QSP_OPMAXARGS];
        QSP_FUNCTION Func;
    } QSPMathOperation;

    enum
    {
        qspOpUnknown,
        qspOpStart,
        qspOpEnd,
        qspOpValue,
        qspOpValueToFormat,
        qspOpOpenArrBracket,
        qspOpCloseArrBracket,
        qspOpOpenBracket,
        qspOpCloseBracket,
        qspOpNegation,
        qspOpComma,
        qspOpMul,
        qspOpDiv,
        qspOpAdd,
        qspOpSub,
        qspOpMod,
        qspOpNe,
        qspOpLeq,
        qspOpGeq,
        qspOpEq,
        qspOpLt,
        qspOpGt,
        qspOpAnd,
        qspOpOr,
        qspOpAppend,

        qspOpFirst_Function,
        qspOpArrItem = qspOpFirst_Function,
        qspOpLastArrItem,
        qspOpNot,
        qspOpLoc,
        qspOpObj,
        qspOpMin,
        qspOpMax,
        qspOpRand,
        qspOpIIf,
        qspOpRGB,
        qspOpLen,
        qspOpIsNum,
        qspOpLCase,
        qspOpUCase,
        qspOpInput,
        qspOpStr,
        qspOpVal,
        qspOpArrSize,
        qspOpIsPlay,
        qspOpDesc,
        qspOpTrim,
        qspOpGetObj,
        qspOpStrComp,
        qspOpStrFind,
        qspOpStrPos,
        qspOpMid,
        qspOpArrPos,
        qspOpArrComp,
        qspOpInstr,
        qspOpReplace,
        qspOpFunc,
        qspOpDynEval,
        qspOpRnd,
        qspOpCountObj,
        qspOpMsecsCount,
        qspOpQSPVer,
        qspOpUserText,
        qspOpCurLoc,
        qspOpSelObj,
        qspOpSelAct,
        qspOpMainText,
        qspOpStatText,
        qspOpCurActs,

        qspOpLast_Operation
    };

    /* External functions */
    void qspInitMath();
    QSPVariant qspExprValue(QSPString expr);