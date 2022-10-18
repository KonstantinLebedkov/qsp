#include "qsp_LinesOfCode.h"

qsp_LinesOfCode& qsp_LinesOfCode::operator=(const qsp_LinesOfCode& src)
{
    if (!src.Lines.empty())
    {
        for (auto it : src.Lines)
        {
            Lines.push_back(new qsp_LineOfCode(*it));
        }
    }
}

bool qsp_LinesOfCode::ExecuteBlock()
{
    //QSP_BOOL qspExecCodeBlockWithLocals(QSPLineOfCode *s, int startLine, int endLine, int codeOffset, QSPString *jumpTo);
    // this function from Statements.h, .cpp.
    bool isExit;
    int oldRefreshCount = qspRefreshCount; //part of locations.h
    qspAllocateSavedVarsGroup(); //part of variables.h
    isExit = ExecuteCode(s, startLine, endLine, codeOffset, jumpTo);//part of statements.h; arguments - is given by caller; and s - must be "this"
    if (qspRefreshCount != oldRefreshCount || qspErrorNum)//errorNum - part of ErrorHandler.
    {
        qspReleaseSavedVarsGroup(QSP_TRUE);//part of variables
        return QSP_FALSE;
    }
    qspReleaseSavedVarsGroup(QSP_FALSE);//part of variables
    return isExit;
}

bool qsp_LinesOfCode::ExecuteCode()
{
    //QSP_BOOL qspExecCode(QSPLineOfCode *s, int startLine, int endLine, int codeOffset, QSPString *jumpTo) //part of statement.h
    qsp_LinesOfCode* line;
    qsp_string jumpToFake;
    bool uLevel, isExit = false;
    int i, oldRefreshCount = qspRefreshCount, action = qspFlowExecute;
    /* Prepare temporary data */
    if (uLevel = !jumpTo) //пустой пойнтер равен false в данном случае. таким образом если джамп-ту пустой, то ставим флаг ю-левел и берем этот иф.
    {
        jumpToFake = qspNullString;// а он и так пустой, не?
        jumpTo = &jumpToFake; //теперь переданный пойнтер джамп-ту станет не пустым, и ссылается на строку. которая в свою очередь пустая.
    }
    /* Code execution */
    i = startLine; //старт-лайн, переданный аргумент, номер строки в блоке кода, с которого пойдет исполнение.
    while (i < endLine) //эндлайн - переданный аргумент, номер строки в блоке кода, до которого идет исполнение.
        //TODO: вот тут мне непонятно, нахрена слать полный блок, когда можно передать выделенную с него копию? и исполнять её всю? потому как всё одно, новый объект "строки кода" тут создаётся, чуть выше, именуемый line
    {
        line = s + i; //ну и тут делается копия. вот да! нахуа! так сложно! делать то?!
        // хм. а ведь есть решение! можно же забубенить через итератор!!!
        if (codeOffset > 0) //предоставленный параметр. и ЧЁ он значит? он интовый.
        {
            qspRealLine = line->LineNum + codeOffset; // а вот тут не вьехал. это ж некая попытка наебалова: мы в эррор-хендлер сдаем не текущую строку, которую пропускаем в выполнение, а её+оффсет. нахрена?
            if (qspIsDebug && !line->Lines.empty()) // если дебаг + непустая строка, то зовем дебаггер на эту строку. и проверяем торможение. при торможении делаем брейк.
            { // а если не дебаг, то весь код-оффсет сводится к наебалову эррор-хендлера.
                qspCallDebug(line->Str);
                if (qspRefreshCount != oldRefreshCount) break;
            } 
        }
        if (line->IsMultiline)
            isExit = qspExecMultilineCode(s, endLine, codeOffset, jumpTo, &i, &action);//part of statement.h
        else
            isExit = qspExecSinglelineCode(s, endLine, codeOffset, jumpTo, &i, &action);//part of statement.h
        if (qspRefreshCount != oldRefreshCount || qspErrorNum) break;
        if (isExit)
        {
            if (!qspIsEmpty(*jumpTo))
            {
                i = qspSearchLabel(s, startLine, endLine, *jumpTo);
                if (i >= 0)
                {
                    jumpTo->End = jumpTo->Str;
                    continue;
                }
                if (uLevel) qspSetError(QSP_ERR_LABELNOTFOUND);
            }
            break;
        }
        if (action == qspFlowExecute)
        {
            isExit = qspExecString(line, 0, line->StatsCount, jumpTo);
            if (qspRefreshCount != oldRefreshCount || qspErrorNum) break;
            if (isExit)
            {
                if (!qspIsEmpty(*jumpTo))
                {
                    i = qspSearchLabel(s, startLine, endLine, *jumpTo);
                    if (i >= 0)
                    {
                        jumpTo->End = jumpTo->Str;
                        continue;
                    }
                    if (uLevel) qspSetError(QSP_ERR_LABELNOTFOUND);
                }
                break;
            }
            ++i;
        }
        else
            action = qspFlowExecute;
    }
    if (uLevel) qspFreeString(jumpToFake);//ну правильно. а если джамп-ту был передан не пустым пойнтером, то ю-левел был поставлен в фолс и нет смысла освобождать строку. а в нашем случае, тем более смысла нет, ведь у неё локальная область видимости и она не пойнтер на строку.
    return isExit;
}
