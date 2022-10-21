#pragma once
#include "qsp_string.h"

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

class qsp_MathOpName
{
public:
	char Code;
	qsp_string Name;
    qsp_MathOpName(char code, qsp_string name) :Code(code), Name(name) {};
};


