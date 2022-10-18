#include "qsp_statements.h"
#include "qsp_errors.h"

void qsp_statements::Init()
{
	//StatMaxLen = 0; //not found any usage of this var
    int i;
    for (i = 0; i < QSP_STATSLEVELS; ++i) StatsNamesCounts[i]=0;
    AddStatement(StatImplicitStatement, StatementImplicitStatement, 1, 1, {64});
    AddStatement(StatElse, 0, 0, 0, {});
    AddStatement(StatElseIf, 0, 1, 1, {0});
    AddStatement(StatEnd, 0, 0, 0,{});
    AddStatement(StatLocal, 0, 0, 0,{});
    AddStatement(StatSet, 0, 0, 0,{});
    AddStatement(StatIf, 0, 1, 1, {0,});
    AddStatement(StatAct, 0, 1, 2, { 1, 1 });
    AddStatement(StatLoop, 0, 0, 0,{});
    AddStatement(StatAddObj, StatementAddObject, 1, 3, { 1, 1, 0 }); //its member of object
    AddStatement(StatClA, StatementClear, 0, 0,{});
    AddStatement(StatCloseAll, qspStatementCloseFile, 0, 0,{}); 
    AddStatement(StatClose, qspStatementCloseFile, 0, 1, {1,}); //member of playlist
    AddStatement(StatClS, StatementClear, 0, 0,{});
    AddStatement(StatCmdClear, StatementClear, 0, 0,{});
    AddStatement(StatCopyArr, qspStatementCopyArr, 2, 4, { 4, 4, 0, 0 });//member of variables
    AddStatement(StatDelAct, qspStatementDelAct, 1, 1, { 1 }); //member of actions
    AddStatement(StatDelObj, qspStatementDelObj, 1, 1, { 1 });//its member of object
    AddStatement(StatDynamic, StatementDynamic, 1, 20, { 1, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 });
    AddStatement(StatExec, StatementExec, 1, 1, { 1 });
    AddStatement(StatExit, StatementExit, 0, 0,{});
    AddStatement(StatFreeLib, StatementClear, 0, 0,{});
    AddStatement(StatGoSub, StatementGoSub, 1, 20, { 1, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 });
    AddStatement(StatGoTo, StatementGoTo, 1, 20, { 1, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 });
    AddStatement(StatIncLib, qspStatementOpenQst, 1, 1, { 1 }); //member of game.cpp
    AddStatement(StatJump, StatementJump, 1, 1, { 1 });
    AddStatement(StatKillAll, StatementClear, 0, 0,{});
    AddStatement(StatKillObj, qspStatementDelObj, 0, 1, { 0 });//its member of object
    AddStatement(StatKillVar, qspStatementKillVar, 0, 2, { 4, 64 });// member of variables
    AddStatement(StatMenu, qspStatementShowMenu, 1, 3, { 4, 0, 0 });// member of menu
    AddStatement(StatMClear, StatementClear, 0, 0,{});
    AddStatement(StatMNL, StatementAddText, 0, 1, { 1 });
    AddStatement(StatMPL, StatementAddText, 0, 1, { 1 });
    AddStatement(StatMP, StatementAddText, 1, 1, { 1 });
    AddStatement(StatClear, StatementClear, 0, 0,{});
    AddStatement(StatNL, StatementAddText, 0, 1, { 1 });
    AddStatement(StatPL, StatementAddText, 0, 1, { 1 });
    AddStatement(StatP, StatementAddText, 1, 1, { 1 });
    AddStatement(StatMsg, StatementMsg, 1, 1, { 1 });
    AddStatement(StatOpenGame, qspStatementOpenGame, 0, 1, { 1 });//member of game
    AddStatement(StatOpenQst, qspStatementOpenQst, 1, 1, { 1 });
    AddStatement(StatPlay, qspStatementPlayFile, 1, 2, { 1, 0 });
    AddStatement(StatRefInt, qspStatementRefInt, 0, 0,{});
    AddStatement(StatSaveGame, qspStatementSaveGame, 0, 1, { 1 });
    AddStatement(StatSetTimer, qspStatementSetTimer, 1, 1, { 0 });
    AddStatement(StatShowActs, qspStatementShowWin, 1, 1, { 0 });
    AddStatement(StatShowInput, qspStatementShowWin, 1, 1, { 0 });
    AddStatement(StatShowObjs, qspStatementShowWin, 1, 1, { 0 });
    AddStatement(StatShowVars, qspStatementShowWin, 1, 1, { 0 });
    AddStatement(StatUnSelect, qspStatementUnSelect, 0, 0,{});
    AddStatement(StatView, qspStatementView, 0, 1, { 1 });
    AddStatement(StatWait, qspStatementWait, 1, 1, { 0 });
    AddStatement(StatXGoTo, StatementGoTo, 1, 20, { 1, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64 });
    /* Names */
    AddStatName(StatElse, L"ELSE", 2);
    AddStatName(StatElseIf, L"ELSEIF", 1);
    AddStatName(StatEnd, L"END", 2);
    AddStatName(StatLocal, L"LOCAL", 2);
    AddStatName(StatSet, L"SET", 2);
    AddStatName(StatSet, L"LET", 2);
    AddStatName(StatIf, L"IF", 2);
    AddStatName(StatAct, L"ACT", 2);
    AddStatName(StatLoop, L"LOOP", 2);
    AddStatName(StatAddObj, L"ADDOBJ", 2);
    AddStatName(StatAddObj, L"ADD OBJ", 2);
    AddStatName(StatClA, L"CLA", 2);
    AddStatName(StatCloseAll, L"CLOSE ALL", 1);
    AddStatName(StatClose, L"CLOSE", 2);
    AddStatName(StatClS, L"CLS", 2);
    AddStatName(StatCmdClear, L"CMDCLEAR", 2);
    AddStatName(StatCmdClear, L"CMDCLR", 2);
    AddStatName(StatCopyArr, L"COPYARR", 2);
    AddStatName(StatDelAct, L"DELACT", 2);
    AddStatName(StatDelAct, L"DEL ACT", 2);
    AddStatName(StatDelObj, L"DELOBJ", 2);
    AddStatName(StatDelObj, L"DEL OBJ", 2);
    AddStatName(StatDynamic, L"DYNAMIC", 2);
    AddStatName(StatExec, L"EXEC", 2);
    AddStatName(StatExit, L"EXIT", 2);
    AddStatName(StatFreeLib, L"FREELIB", 2);
    AddStatName(StatGoSub, L"GOSUB", 2);
    AddStatName(StatGoSub, L"GS", 2);
    AddStatName(StatGoTo, L"GOTO", 2);
    AddStatName(StatGoTo, L"GT", 2);
    AddStatName(StatIncLib, L"INCLIB", 2);
    AddStatName(StatJump, L"JUMP", 2);
    AddStatName(StatKillAll, L"KILLALL", 2);
    AddStatName(StatKillObj, L"KILLOBJ", 2);
    AddStatName(StatKillVar, L"KILLVAR", 2);
    AddStatName(StatMenu, L"MENU", 2);
    AddStatName(StatMClear, L"*CLEAR", 2);
    AddStatName(StatMClear, L"*CLR", 2);
    AddStatName(StatMNL, L"*NL", 2);
    AddStatName(StatMPL, L"*PL", 1);
    AddStatName(StatMP, L"*P", 2);
    AddStatName(StatClear, L"CLEAR", 2);
    AddStatName(StatClear, L"CLR", 2);
    AddStatName(StatNL, L"NL", 2);
    AddStatName(StatPL, L"PL", 1);
    AddStatName(StatP, L"P", 2);
    AddStatName(StatMsg, L"MSG", 2);
    AddStatName(StatOpenGame, L"OPENGAME", 2);
    AddStatName(StatOpenQst,L"OPENQST", 2);
    AddStatName(StatPlay, L"PLAY", 0);
    AddStatName(StatRefInt, L"REFINT", 2);
    AddStatName(StatSaveGame, L"SAVEGAME", 2);
    AddStatName(StatSetTimer, L"SETTIMER", 1);
    AddStatName(StatShowActs, L"SHOWACTS", 2);
    AddStatName(StatShowInput, L"SHOWINPUT", 2);
    AddStatName(StatShowObjs, L"SHOWOBJS", 2);
    AddStatName(StatShowVars, L"SHOWSTAT", 2);
    AddStatName(StatUnSelect, L"UNSELECT", 1);
    AddStatName(StatUnSelect, L"UNSEL", 2);
    AddStatName(StatView, L"VIEW", 2);
    AddStatName(StatWait, L"WAIT", 2);
    AddStatName(StatXGoTo, L"XGOTO", 2);
    AddStatName(StatXGoTo, L"XGT", 2);
    for (i = 0; i < QSP_STATSLEVELS; ++i)
        qsort(StatsNames[i], StatsNamesCounts[i], sizeof(qsp_StatName), StatsCompare);
    // и тут внимание! есть подозрение что этот сортировщик вызваемый по массиву, может бросить ошибку, так как ему зачем-то нужен размер стат-нейма в этом массиве. и тут вопрос, если там стринг лежит пойнтером, то пофиг. а вот если телом - то размер то разный!
}
qsp_variants qsp_statements::GetStatArgs(qsp_string s, qsp_CachedStat* stat)
{
    qsp_variants args;
    //интом возвращает число обработанных аргументов.
    if (stat->ErrorCode)
    {
        SetError(stat->ErrorCode);
        return args;
    }
    if (stat->Args.size() > 0)
    {//если число аргументов в кешед-стат больше нуля, 
        char type;
        int ArgNumber=0, oldRefreshCount = qspRefreshCount;
        for (auto it: stat->Args)
        { // делаем цикл по их числу
            args.push_back( qspExprValue(qspStringFromPair(s.Str + it->StartPos, s.Str + it->EndPos)) ); //в аргс по индексу пишем значение, вытянутое из строки по индексам начала и конца аргумента.
            if (qspRefreshCount != oldRefreshCount || qspErrorNum)
            {
                args.ClearVariants();
                return args;
            }
            if ((Stats[stat->Stat].ArgsTypes[ArgNumber++] != QSP_TYPE_UNDEFINED) && !qspConvertVariantTo(args.rbegin(), type))
            { // если тайп неопределенный, и не получается конвертить - прекращаем цикл, убиваем обработанные аргументы и кидаем результат "обработанных аргументов нуль"
                SetError(QSP_ERR_TYPEMISMATCH);
                args.ClearVariants();
                return args;
            }
        }
    }
    return args;
}
;

void qsp_statements::AddStatement(char statCode, qsp_STATEMENT func, char minArgs, char maxArgs, std::vector<char>args)
{
    Stats[statCode].Func = func;
    Stats[statCode].MinArgsCount = minArgs;
    Stats[statCode].MaxArgsCount = maxArgs;
    Stats[statCode].ArgsTypes = args;
};

int qsp_statements::StatsCompare(const void* statName1, const void* statName2)
{
    return qspStrsComp(((qsp_StatName*)statName1)->Name, ((qsp_StatName*)statName2)->Name);
};

void qsp_statements::AddStatName(char statCode, qsp_string statName, int level)
{
    int count, len = statName.length();
    count = StatsNamesCounts[level];
    StatsNames[level][count].Name = statName;
    StatsNames[level][count].Code = statCode;
    StatsNamesCounts[level] = count + 1;
    /* Max length */
    //if (len > qspStatMaxLen) qspStatMaxLen = len; //not found any usage of variable "qspStatMaxLen"
};

void qsp_statements::AddStatName(char statCode, std::wstring statName, int level)
{
    int count, len = statName.length();
    count = StatsNamesCounts[level];
    StatsNames[level][count].Name = statName;
    StatsNames[level][count].Code = statCode;
    StatsNamesCounts[level] = count + 1;
    /* Max length */
    //if (len > qspStatMaxLen) qspStatMaxLen = len; //not found any usage of variable "qspStatMaxLen"
};

bool qsp_statements::StatementImplicitStatement(qsp_variant* args, char count, qsp_string* jumpTo, char extArg)
//TODO: replace func as member of common
{
    if (args[0].Type != QSP_TYPE_UNDEFINED)
    {
        qspConvertVariantTo(args, QSP_TYPE_STRING);
        //TODO: и вот задача. нужно для варианта реализовать метод конвертации в.
        //в данном случае - в строку. 
        qspCurDesc.append(args[0].Val.Str);//common - CurDesc
        qspCurDesc.append(L"\r\n");
        qspIsMainDescChanged = true;
    }
    return false;
};

bool qsp_statements::StatementClear(qsp_variant* args, char count, qsp_string* jumpTo, char extArg)
{
    switch (extArg)
    {
    case StatClear:
        if (qspClearText(&qspCurVars)) //in text : QSP_BOOL qspClearText(QSPString *s); for CurVars, member of common
            qspIsVarsDescChanged = true; // member of common
        break;
    case StatMClear:
        if (qspClearText(&qspCurDesc)) //in text : QSP_BOOL qspClearText(QSPString *s); for CurVars, member of common
            qspIsMainDescChanged = true; // member of common
        break;
        break; // why double break?!
    case StatCmdClear:
        qspClearText(&qspCurInput); //in text : QSP_BOOL qspClearText(QSPString *s); for CurVars, member of common
        qspCallSetInputStrText(qspNullString); //it is in callbacks
        break;
    case StatClA:
        qspClearActions(false); //it is in actions?
        break;
    case StatClS:
        if (qspClearText(&qspCurVars)) //in text : QSP_BOOL qspClearText(QSPString *s); for CurVars, member of common
            qspIsVarsDescChanged = true;// member of common
        if (qspClearText(&qspCurDesc))
            qspIsMainDescChanged = true;
        qspClearText(&qspCurInput);
        qspClearActions(false);
        qspCallSetInputStrText(qspNullString);
        break;
    case StatKillAll:
        qspClearVars(false);
        qspClearObjectsWithNotify();
        break;
    case StatFreeLib:
        qspClearIncludes(false);
        if (qspCurLoc >= qspLocsCount) qspCurLoc = -1;
        break;
    }
    return false;
};

bool qsp_statements::StatementDynamic(qsp_variant* args, char count, qsp_string* jumpTo, char extArg)
{
    qspExecStringAsCodeWithArgs(QSP_STR(args[0]), args + 1, count - 1, 0, 0);
    return false;
};

bool qsp_statements::StatementExec(qsp_variant* args, char count, qsp_string* jumpTo, char extArg)
{
    qspCallSystem(QSP_STR(args[0]));
    return QSP_FALSE;
};

bool qsp_statements::StatementExit(qsp_variant* args, char count, qsp_string* jumpTo, char extArg)
{
    return true;
};

bool qsp_statements::StatementGoSub(qsp_variant* args, char count, qsp_string* jumpTo, char extArg)
{
    qspExecLocByNameWithArgs(QSP_STR(args[0]), args + 1, count - 1, 0);
    return false;
};

bool qsp_statements::StatementGoTo(qsp_variant* args, char count, qsp_string* jumpTo, char extArg)
{
    int locInd = qspLocIndex(QSP_STR(args[0]));
    if (locInd < 0)
    {
        qspSetError(QSP_ERR_LOCNOTFOUND);
        return false;
    }
    qspCurLoc = locInd;
    qspRefreshCurLoc(extArg == qspStatGoTo, args + 1, count - 1);
    return false;
};

bool qsp_statements::StatementJump(qsp_variant* args, char count, qsp_string* jumpTo, char extArg)
{
    qspUpdateText(jumpTo, qspDelSpc(QSP_STR(args[0])));
    qspUpperStr(jumpTo);
    return true;
};

bool qsp_statements::StatementAddText(qsp_variant* args, char count, qsp_string* jumpTo, char extArg)
// summary, it must call for methods of "common" or "game"
{
    switch (extArg)
    {
    case StatP:
        if (!qspIsEmpty(QSP_STR(args[0])))
        {
            qspCurVars.append(QSP_STR(args[0]));//was: qspAddText(&qspCurVars, QSP_STR(args[0]), QSP_FALSE);
            qspIsVarsDescChanged = QSP_TRUE;
        }
        break;
    case StatMP:
        if (!qspIsEmpty(QSP_STR(args[0])))
        {
            qspCurDesc.append(QSP_STR(args[0]));//was: qspAddText(&qspCurDesc, QSP_STR(args[0]), QSP_FALSE);
            qspIsMainDescChanged = QSP_TRUE;
        }
        break;
    case StatPL:
        if (count) qspCurVars.append(QSP_STR(args[0]));//was: qspAddText(&qspCurVars, QSP_STR(args[0]), QSP_FALSE);
        qspCurVars.append(L"\r\n");//was: qspAddText(&qspCurVars, QSP_STATIC_STR(QSP_STRSDELIM), QSP_FALSE);
        qspIsVarsDescChanged = QSP_TRUE;
        break;
    case StatMPL:
        if (count) qspCurDesc.append(QSP_STR(args[0]));//was: qspAddText(&qspCurDesc, QSP_STR(args[0]), QSP_FALSE);
        qspCurDesc.append(L"\r\n");//was: qspAddText(&qspCurDesc, QSP_STATIC_STR(QSP_STRSDELIM), QSP_FALSE);
        qspIsMainDescChanged = QSP_TRUE;
        break;
    case StatNL:
        qspCurVars.append(L"\r\n");//was: qspAddText(&qspCurVars, QSP_STATIC_STR(QSP_STRSDELIM), QSP_FALSE);
        if (count) qspCurVars.append(QSP_STR(args[0]));//was: qspAddText(&qspCurVars, QSP_STR(args[0]), QSP_FALSE);
        qspIsVarsDescChanged = QSP_TRUE;
        break;
    case StatMNL:
        qspCurDesc.append(L"\r\n");//was: qspAddText(&qspCurDesc, QSP_STATIC_STR(QSP_STRSDELIM), QSP_FALSE);
        if (count) qspCurDesc.append(QSP_STR(args[0]));//was: qspAddText(&qspCurDesc, QSP_STR(args[0]), QSP_FALSE);
        qspIsMainDescChanged = QSP_TRUE;
        break;
    }
    return false;
};

bool qsp_statements::StatementMsg(qsp_variant* args, char count, qsp_string* jumpTo, char extArg)
{
    qspCallShowMessage(QSP_STR(args[0]));
    return false;
};