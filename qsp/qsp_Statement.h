#pragma once
#include "qsp_string.h"
#include "qsp_variants.h"
#include <functional>


#define QSP_STATSLEVELS 3
#define QSP_STATMAXARGS 20
#define QSP_MAXSTATSNAMES 100

enum
{
StatUnknown,
StatLabel,
StatComment,
StatImplicitStatement,
StatAct,
StatLoop,
StatLocal,
StatIf,
StatElseIf,
StatElse,
StatEnd,
StatAddObj,
StatClA,
StatClear,
StatCloseAll,
StatClose,
StatClS,
StatCmdClear,
StatCopyArr,
StatDelAct,
StatDelObj,
StatDynamic,
StatExec,
StatExit,
StatFreeLib,
StatGoSub,
StatGoTo,
StatIncLib,
StatJump,
StatKillAll,
StatKillObj,
StatKillVar,
StatMClear,
StatMenu,
StatMNL,
StatMPL,
StatMP,
StatMsg,
StatNL,
StatOpenGame,
StatOpenQst,
StatPlay,
StatPL,
StatP,
StatRefInt,
StatSaveGame,
StatSetTimer,
StatSet,
StatShowActs,
StatShowInput,
StatShowObjs,
StatShowVars,
StatUnSelect,
StatView,
StatWait,
StatXGoTo,

StatLast_Statement
};

using qsp_STATEMENT = bool (*) (qsp_variants args, char count, qsp_string* jumpTo, char extArg);
//подозреваю, что корректным будет "вектор вариантов, чар, вектор строк, чар)
//TODO: may be that will be more correct to remane from "qsp_STATEMENT" to "qsp_func" and extract it to own file
class qsp_Statement
{
public:
    char MinArgsCount;
    char MaxArgsCount;
    std::vector<char> ArgsTypes;
    qsp_STATEMENT Func;

};

