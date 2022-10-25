#include "qsp_MathOpNames.h"

char qsp_MathOpNames::OperatorCode(qsp_string* expr)
{
    int i;
    qsp_MathOpName* name;
    if (expr->empty()) return qspOpEnd;
    for (i = 0; i < QSP_OPSLEVELS; ++i)
    { //а тут поиск по сортированному вектору this, и если найдено имя, то возвращаем номер (или итератор)
        // но не ясно мне, набуа разделены в 3 слоя имена?
        name = (qsp_MathOpName*)bsearch(this, qspOpsNames[i], qspOpsNamesCounts[i], sizeof(QSPMathOpName), qspMathOpStringCompare);
        if (name)
        {
            expr->Str += qspStrLen(name->Name);
            return name->Code;
        }
    }
    return qspOpUnknown;
};

qsp_MathOpNames::iterator qsp_MathOpNames::GetCodeByName(qsp_string name)
{
    // of course, we can use std::Find_if, but! it only compare and do it "for each"
    //TODO: optimise with binary search
    return std::find_if(this->begin(), this->end(), [&](qsp_MathOpNames::iterator n) {return name.compare((*n)->Name); });
};
void qsp_MathOpNames::Clean()
{
    for (auto it : *this) 
        it->~qsp_MathOpName(); 
    this->clear();
};
