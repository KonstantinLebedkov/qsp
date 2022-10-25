#include "qsp_MathOps.h"

void qsp_MathOps::AddOperation(char opCode, char priority, qsp_Function func, char resType, char minArgs, char maxArgs, std::vector<char>args)
{
    mathOperations.AddOperation(opCode, priority, func, resType, minArgs, maxArgs,args);
}

void qsp_MathOps::AddOpName(char opCode, String opName, int level)
{
    MathOpNames.at(level).push_back(new qsp_MathOpName(opCode, opName));
}

char qsp_MathOps::functionOpCode(String funName)
{// i didnt understand, why we search only in last layer of array(in my case, vector) but i'll still it as exist.
    auto name = MathOpNames.rbegin()->GetCodeByName(funName);
    if (name != MathOpNames.rbegin()->end()) return (*name)->Code;
    return qspOpUnknown;
};

int qsp_MathOps::GetNumber(String* expr)
{
    String::size_type tail;
    int num = std::stoi(*expr, &tail);
    expr->assign(expr->substr(tail));
    if (num < 0) return INT_MAX; /* simple overflow protection */
    return num;
};

String qsp_MathOps::GetName(String* expr)
{
    SIterator endPos = expr->begin();
    while (++endPos < expr->end())//move iterator to next, until end of string
    {
        if (*endPos<128 && ((qspAsciiClasses[*endPos] & 4) != 0)) 
                break; //do break this "while" if EOT found
    }
    String result = expr->substr(0, distance(expr->begin(), endPos));
    expr->assign(expr->substr(distance(expr->begin(),endPos)));
    return result;
};


char qsp_MathOps::OperatorOpCode(String* expr)
{
    if (expr->empty()) return qspOpEnd;
    for (qsp_MathOpNames Names : MathOpNames)
    {
        auto name = (Names).GetCodeByName(*expr);
        if (name != (Names).end())
        {
            expr->assign(expr->substr((*name)->Name.length()));
            return (*name)->Code;
        }
    }
    return qspOpUnknown;
};

String qsp_MathOps::GetQString(String* expr)
{
    SIterator pos;
    pos = expr->DelimPos(L'}');
    if (pos == expr->end())
    {
        qspSetError(QSP_ERR_QUOTNOTFOUND);
        return String(L'');
    }
    String result = expr->substr(1, distance(expr->begin(), pos) - 1);
    expr->assign(expr->substr(distance(pos, expr->begin()) + 1));
    return result;
};

int qsp_MathOps::FreeValue(int valueIndex, qsp_variant* compValues, std::vector<char> compOpCodes, std::vector<char> compArgsCounts)
{
    char argsCount;
    if (valueIndex < 0) return -1;
    argsCount = compArgsCounts[valueIndex]; //no need for dynamic array
    if (argsCount) //���� ����-����� �� ����
    {
        int i;
        --valueIndex; //�� ���� �� ����-����� ����������, ��� �����-������ ������������� �� ������� ������ (� ��������, � �������� �� ���� ����� ��������������. ��� ������ ������� �����-��!!! �������� ��� ������, ������! ��� ��������� ������� ��-�������! 
        for (i = 0; i < argsCount; ++i)//reqursive call for themself
            valueIndex = qspFreeValue(valueIndex, compValues, compOpCodes, compArgsCounts);
    }
    else
    {// � ���� ����-����� ��� �����-������ ���� ����, ��!
        switch (compOpCodes[valueIndex])
        {//�������� �� ��-����
        case qspOpValue:
        case qspOpValueToFormat:
            if (QSP_ISSTR(compValues[valueIndex].Type)) qspFreeString(QSP_STR(compValues[valueIndex]));
            break;
        }
        //���� ��-��� �������� "�����������" ��� "�������������������", ������ ��.
        --valueIndex;//�������� �����-������� �����������, � ������� �� �������� 
    }
    return valueIndex; //���������, ��� ����, ����������� � ���������� �������� �����-������
};

int qsp_MathOps::SkipValue(int valueIndex, char* compArgsCounts)
{
    char argsCount;
    if (valueIndex < 0) return -1; //��������� ��������, ���� ����� �� ��� �������
    argsCount = compArgsCounts[valueIndex];
    --valueIndex; //����������� ������
    if (argsCount)
    { //���� � ������� ����� �� ������ ��������� �� ���� 
        int i;
        for (i = 0; i < argsCount; ++i) //�� ������ ������� ����������� �������, ������� ������� � �������
            valueIndex = qspSkipValue(valueIndex, compArgsCounts);
    }
    return valueIndex; // � ���������� ���� ����� ���� ��������.
}
qsp_MathOps& qsp_MathOps::MathOpsHandler()
{
    static qsp_MathOps theInstance;
    return theInstance;
}
;
