/* Copyright (C) 2001-2020 Valeriy Argunov (byte AT qsp DOT org) */
/* Copyright (C) 2022 Konstantin Lebedkov*/

#include "actions.h"
#include "errors.h"
#include "game.h"
#include "locations.h"
#include "statements.h"
#include "text.h"

QSPCurAct qspCurActions[QSP_MAXACTIONS];

INLINE int qspActIndex(QSPString name);

void Actions::ClearActions(bool isFirst)
{
    if (!isFirst) {
        List.clear();
        IsActionsChanged = true;
    }
    CurActionsCount = 0;
    CurSelAction = -1;
}

void Actions::AddAction(QSPVariant* args, char count, QSPString * code, int start, int end)
{
    if (ActIndex(QSP_STR(args[0])) != nullptr) return; //проверка, что такого акшена ещЄ нет в листе. но если он обнаружен, тоесть ссылка на него непуста€, то выход из процедуры.
    if (qspCurActionsCount == QSP_MAXACTIONS)
    // провер€етс€, что список акшенов не превышает допустимого числа. иначе процедура обрываетс€ выдав ошибку
    {
        qspSetError(QSP_ERR_CANTADDACTION);
        return;
    }
    QSPString imgPath;
    // а вот ниже по вилке идет присвоение значени€ в строку. и вот ну ЄмаЄ, тернарник напрашиваетс€, да.
                //в принципе, этот тернарник уйдет в конструктор.
    
    //act = qspCurActions + qspCurActionsCount++; //здесь пойнтерной математикой сдвинулись на нужный акт и далее заполн€ем
    //а поскольку мы сдвигать нифига не будем, то просто пуш-бек в вектор нового акшена.
    List.push_back(new Action(QSP_STR(args[0]), (count == 2 && qspIsAnyString(QSP_STR(args[1]))) ? qspGetNewText(QSP_STR(args[1])) : ""; , qspCopyPrepLines(&act->OnPressLines, code, start, end)))
    //TODO: поставить локейшен и акт индекс в конструктор, если вы€снитс€ что оне нужны. и тогда поставить в дефолт конструктор им заглушки.
    act->OnPressLinesCount = end - start;
    act->Location = qspRealCurLoc;
    act->ActIndex = qspRealActIndex;
    IsActionsChanged = true;
}


Action * Actions::ActIndex(QSPString name)
// поиск акшена по его имени, и возврат указател€ на него.
// должно быть нормально.
{
    if (List.size()==0) return nullptr;
    for (auto Act :List )
    {
        if (Act->GetDesc() == name) return Act;
    }
    return nullptr;
}

void Actions::ExecuteAction(int ind)
{
    if (ind >= 0 && ind < CurActionsCount)
    {
        int count;
        QSPString * code;
        std::vector<Action*>::iterator Act = List.begin() + ind;
        /* switch the current state */
        qspRealCurLoc = Act->Location;
        qspRealActIndex = act->ActIndex;
        count = act->OnPressLinesCount;
        qspCopyPrepLines(&code, act->OnPressLines, 0, count);
        qspExecCodeBlockWithLocals(code, 0, count, 1, 0);
        qspFreePrepLines(code, count);
    }
}

QSPString qspGetAllActionsAsCode()
{
    int count, i;
    QSPString temp, res;
    res = qspNullString;
    for (i = 0; i < qspCurActionsCount; ++i)
    {
        qspAddText(&res, "ACT '", QSP_FALSE);
        temp = qspReplaceText(qspCurActions[i].Desc, "'", "''");
        qspAddText(&res, temp, QSP_FALSE);
        qspFreeString(temp);
        if (qspCurActions[i].Image.Str)
        {
            qspAddText(&res, "','", QSP_FALSE);
            temp = qspReplaceText(qspCurActions[i].Image, "'", "''");
            qspAddText(&res, temp, QSP_FALSE);
            qspFreeString(temp);
        }
        qspAddText(&res, "':", QSP_FALSE);
        count = qspCurActions[i].OnPressLinesCount;
        if (count == 1 && qspIsAnyString(qspCurActions[i].OnPressLines->Str))
            qspAddText(&res, qspCurActions[i].OnPressLines->Str, QSP_FALSE);
        else
        {
            if (count >= 2)
            {
                qspAddText(&res, QSP_STATIC_STR(QSP_STRSDELIM), QSP_FALSE);
                temp = qspJoinPrepLines(qspCurActions[i].OnPressLines, count, QSP_STATIC_STR(QSP_STRSDELIM));
                qspAddText(&res, temp, QSP_FALSE);
                qspFreeString(temp);
            }
            qspAddText(&res, QSP_STATIC_STR(QSP_STRSDELIM QSP_FMT("END")), QSP_FALSE);
        }
        qspAddText(&res, QSP_STATIC_STR(QSP_STRSDELIM), QSP_FALSE);
    }
    return res;
}

void qspStatementSinglelineAddAct(QSPLineOfCode *s, int statPos, int endPos)
{
    QSPVariant args[2];
    QSPLineOfCode code;
    int i, j, oldRefreshCount, count, offset;
    QSP_CHAR *lastPos, *firstPos = s->Str.Str + s->Stats[statPos].EndPos;
    if (firstPos == s->Str.End || *firstPos != QSP_COLONDELIM[0])
    {
        qspSetError(QSP_ERR_COLONNOTFOUND);
        return;
    }
    if (statPos == endPos - 1)
    {
        qspSetError(QSP_ERR_CODENOTFOUND);
        return;
    }
    oldRefreshCount = qspRefreshCount;
    count = qspGetStatArgs(s->Str, s->Stats + statPos, args);
    if (qspRefreshCount != oldRefreshCount || qspErrorNum)
        return;
    ++statPos; /* start with the internal code */
    firstPos += QSP_STATIC_LEN(QSP_COLONDELIM);
    lastPos = s->Str.Str + s->Stats[endPos - 1].EndPos;
    if (lastPos != s->Str.End && *lastPos == QSP_COLONDELIM[0]) lastPos += QSP_STATIC_LEN(QSP_COLONDELIM);
    code.Str = qspStringFromPair(firstPos, lastPos);
    code.Label = qspGetLineLabel(code.Str);
    code.LineNum = s->LineNum;
    code.IsMultiline = QSP_FALSE;
    code.StatsCount = endPos - statPos;
    code.Stats = (QSPCachedStat *)malloc(code.StatsCount * sizeof(QSPCachedStat));
    offset = (int)(firstPos - s->Str.Str);
    for (i = 0; i < code.StatsCount; ++i)
    {
        code.Stats[i].Stat = s->Stats[statPos].Stat;
        code.Stats[i].ParamPos = s->Stats[statPos].ParamPos - offset;
        code.Stats[i].EndPos = s->Stats[statPos].EndPos - offset;
        code.Stats[i].ErrorCode = s->Stats[statPos].ErrorCode;
        code.Stats[i].ArgsCount = s->Stats[statPos].ArgsCount;
        code.Stats[i].Args = (QSPCachedArg *)malloc(code.Stats[i].ArgsCount * sizeof(QSPCachedArg));
        for (j = 0; j < code.Stats[i].ArgsCount; ++j)
        {
            code.Stats[i].Args[j].StartPos = s->Stats[statPos].Args[j].StartPos - offset;
            code.Stats[i].Args[j].EndPos = s->Stats[statPos].Args[j].EndPos - offset;
        }
        ++statPos;
    }
    qspAddAction(args, count, &code, 0, 1);
    qspFreeVariants(args, count);
    qspFreeLineOfCode(&code);
}

void qspStatementMultilineAddAct(QSPLineOfCode *s, int lineInd, int endLine)
{
    QSPVariant args[2];
    int count, oldRefreshCount;
    QSPLineOfCode *line = s + lineInd;
    oldRefreshCount = qspRefreshCount;
    count = qspGetStatArgs(line->Str, line->Stats, args);
    if (qspRefreshCount != oldRefreshCount || qspErrorNum) return;
    qspAddAction(args, count, s, lineInd + 1, endLine);
    qspFreeVariants(args, count);
}

QSP_BOOL qspStatementDelAct(QSPVariant *args, QSP_TINYINT count, QSPString *jumpTo, QSP_TINYINT extArg)
{
    int actInd = qspActIndex(QSP_STR(args[0]));
    if (actInd < 0) return QSP_FALSE;
    if (qspCurSelAction >= actInd) qspCurSelAction = -1;
    qspFreeString(qspCurActions[actInd].Image);
    qspFreeString(qspCurActions[actInd].Desc);
    qspFreePrepLines(qspCurActions[actInd].OnPressLines, qspCurActions[actInd].OnPressLinesCount);
    --qspCurActionsCount;
    while (actInd < qspCurActionsCount)
    {
        qspCurActions[actInd] = qspCurActions[actInd + 1];
        ++actInd;
    }
    qspIsActionsChanged = QSP_TRUE;
    return QSP_FALSE;
}
