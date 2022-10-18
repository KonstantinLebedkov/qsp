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
    if (uLevel = !jumpTo) //������ ������� ����� false � ������ ������. ����� ������� ���� �����-�� ������, �� ������ ���� �-����� � ����� ���� ��.
    {
        jumpToFake = qspNullString;// � �� � ��� ������, ��?
        jumpTo = &jumpToFake; //������ ���������� ������� �����-�� ������ �� ������, � ��������� �� ������. ������� � ���� ������� ������.
    }
    /* Code execution */
    i = startLine; //�����-����, ���������� ��������, ����� ������ � ����� ����, � �������� ������ ����������.
    while (i < endLine) //������� - ���������� ��������, ����� ������ � ����� ����, �� �������� ���� ����������.
        //TODO: ��� ��� ��� ���������, ������� ����� ������ ����, ����� ����� �������� ���������� � ���� �����? � ��������� � ���? ������ ��� �� ����, ����� ������ "������ ����" ��� ��������, ���� ����, ��������� line
    {
        line = s + i; //�� � ��� �������� �����. ��� ��! �����! ��� ������! ������ ��?!
        // ��. � ���� ���� �������! ����� �� ���������� ����� ��������!!!
        if (codeOffset > 0) //��������������� ��������. � ר �� ������? �� �������.
        {
            qspRealLine = line->LineNum + codeOffset; // � ��� ��� �� ������. ��� � ����� ������� ���������: �� � �����-������� ����� �� ������� ������, ������� ���������� � ����������, � �+������. �������?
            if (qspIsDebug && !line->Lines.empty()) // ���� ����� + �������� ������, �� ����� �������� �� ��� ������. � ��������� ����������. ��� ���������� ������ �����.
            { // � ���� �� �����, �� ���� ���-������ �������� � ��������� �����-��������.
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
    if (uLevel) qspFreeString(jumpToFake);//�� ���������. � ���� �����-�� ��� ������� �� ������ ���������, �� �-����� ��� ��������� � ���� � ��� ������ ����������� ������. � � ����� ������, ��� ����� ������ ���, ���� � �� ��������� ������� ��������� � ��� �� ������� �� ������.
    return isExit;
}
