#include "qsp_LineOfCode.h"


qsp_LineOfCode& qsp_LineOfCode::operator=(const qsp_LineOfCode& src)
{
    this->Str = src.Str;
    this->LineNum = src.LineNum;
    if (!src.Stats.empty())
    {
        for (auto it : src.Stats)
        {
            Stats.push_back(new qsp_CachedStat(*it));
        }
    }
    this->IsMultiline = src.IsMultiline;
    this->Label = src.Label;
    return *this;
};

qsp_LineOfCode::~qsp_LineOfCode()
{
    Str.~qsp_string();
    Label.~qsp_string();
    for (auto it : Stats)
        it->~qsp_CachedStat();
};

bool qsp_LineOfCode::ExecuteMultilineCode(qsp_LineOfCode* s, int endLine, int codeOffset, qsp_string* jumpTo, int* lineInd, int* action)
{
    qsp_LineOfCode* line; //должен быть итератор. или это "this"?
    int ind = *lineInd;
    endLine = qspSearchEnd(s, ind + 1, endLine);//part of statement.h //ищет номер строки, в которой есть энд, закрывающий блок кода. по идее, блок кода должен начинаться со строки старт. и тут вижу возможность бага.
    if (endLine < 0)
    {
        qsp_errors::ErrorsHandler();
        qspSetError(QSP_ERR_ENDNOTFOUND);
        return false;
    }
    line = s + ind;//там внутри вектор, и потому работать надо через итератор. и тут вопрос, это должно быть метод для Line или Lines? по идее, мультилайн - для Lines
    //statCode = line->Stats[0]->Stat; //а там статов - вектор! и по дефолту, нулевой что ли?
    switch (line->Stats[0]->Stat)
    {
    case qspStatIf:
    case qspStatElseIf:
    {
        int elsePos, oldRefreshCount = qspRefreshCount;
        bool condition = qspCheckCondition(qspStringFromPair(line->Str.Str + line->Stats->ParamPos, line->Str.Str + line->Stats->EndPos));
        if (qspRefreshCount != oldRefreshCount || qspErrorNum) return false;
        elsePos = qspSearchElse(s, ind + 1, endLine);
        if (condition)
        {
            *lineInd = endLine;
            *action = qspFlowJumpToSpecified;
            if (elsePos >= 0)
                return qspExecCodeBlockWithLocals(s, ind + 1, elsePos, codeOffset, jumpTo);
            return qspExecCodeBlockWithLocals(s, ind + 1, endLine, codeOffset, jumpTo);
        }
        else
        {
            *lineInd = (elsePos >= 0 ? elsePos : endLine);
            *action = qspFlowJumpToSpecified;
        }
        break;
    }
    case qspStatAct:
        *lineInd = endLine;
        *action = qspFlowJumpToSpecified;
        qspStatementMultilineAddAct(s, ind, endLine);
        break;
    case qspStatLoop:
        *lineInd = endLine;
        *action = qspFlowJumpToSpecified;
        return qspStatementMultilineLoop(s, ind, endLine, codeOffset, jumpTo);
    }
    return false;
};