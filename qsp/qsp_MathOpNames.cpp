#include "qsp_MathOpNames.h"

char qsp_MathOpNames::OperatorCode(qsp_string* expr)
{
    int i;
    qsp_MathOpName* name;
    if (expr->empty()) return qspOpEnd;
    for (i = 0; i < QSP_OPSLEVELS; ++i)
    { //� ��� ����� �� �������������� ������� this, � ���� ������� ���, �� ���������� ����� (��� ��������)
        // �� �� ���� ���, ����� ��������� � 3 ���� �����?
        name = (qsp_MathOpName*)bsearch(this, qspOpsNames[i], qspOpsNamesCounts[i], sizeof(QSPMathOpName), qspMathOpStringCompare);
        if (name)
        {
            expr->Str += qspStrLen(name->Name);
            return name->Code;
        }
    }
    return qspOpUnknown;
