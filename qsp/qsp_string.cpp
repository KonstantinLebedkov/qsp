#include <cwctype>
#include "qsp_string.h"
#include "qsp_variants.h"
#include "qsp_MathOps.h"
#include "qsp_expression.h"
#include "qsp_errors.h"

bool qsp_string::replaceEntryText(const String from, const String to) {
    size_t start_pos = this->find(from);
    if (start_pos == String::npos)
        return false;
    this->replace(start_pos, from.length(), to);
    return true;
};

bool qsp_string::replaceEntryText(const std::wstring from, const std::wstring to) {
    size_t start_pos = this->find(from);
    if (start_pos == String::npos)
        return false;
    this->replace(start_pos, from.length(), to);
    return true;
};

void qsp_string::SkipSpaces()
{
    while ( IsInClass( QSP_CHAR_SPACE)) 
        this->assign(this->substr(1));
};

String qsp_string::replaceText(const String from, const String to) {
    String result = String(*this);
    while (result.replaceEntryText(from, to)) {};
    return result;
};

qsp_string qsp_string::replaceText(const std::wstring from, const std::wstring to) {
    String result = String(*this);
    while (result.replaceEntryText(from, to)) {};
    return result;
};

void qsp_string::replaceTextInSelf(const String from, const String to) {
    while (this->replaceEntryText(from,to)){}
};

void qsp_string::replaceTextInSelf(const std::wstring from, const std::wstring to) {
    while (this->replaceEntryText(from, to)) {}
};

bool qsp_string::IsAnyString()
{
    //TODO: IMHO, it is not good to check for spaces here. it must be cleared from spaces during initial parsing and nomore else.
    //TODO: this method called for some variant fields. for example, in qsp.actions->AddAction. for union, it must be checked for field type before, another case may cause error.
    String str(*this);
    str.SkipSpaces();
    return str.empty();
}

int qsp_string::ToInteger()
{
    return std::stoi(*this);
}

int qsp_string::GetNumber()
{
    size_t tail;
    int num = std::stoi(*this, &tail);
    assign(substr(tail));
    if (num < 0) return INT_MAX; /* simple overflow protection */
    return num;
}

void qsp_string::UpperString()
{
    for (auto it : *this) it = std::towupper(it);
}

void qsp_string::LowerString()
{
    for (auto it : *this) it = std::towlower(it);
}

void qsp_string::DeleteSpaces()
{
    SIterator begin = this->begin();
    SIterator end = this->end();
    while (begin < end && *begin != *(L" ")) ++begin;
    while (begin < end && *(end - 1)!= *(L" ")) --end;
    this->assign(this->substr(distance(this->begin(), begin), distance(begin, end)));
}

void qsp_string::ExprValue()
{
    //qsp_variants compValues; //��������� ������ ���������, ������������ 200. �����?
    //std::vector<char> compOpCodes, compArgsCounts; // ��������� 2 ������� �����, ������������ 200 ������.
    qsp_CompiledExpression CompiledExpression = this->CompileExpression();
    if (!CompiledExpression.itemsCount)
        return qspGetEmptyVariant(QSP_TYPE_UNDEFINED); //���� �����-����� �������� �����, ������ ���� - ����������� � ��� � �� ������� ������ �����-�������, � ����� "�����������"
    return qspValue(--itemsCount, compValues, compOpCodes, compArgsCounts); /* the last item represents the whole expression */
}

bool qsp_string::IsDigit()
{
    return (this->at(0) >= L'0' && this->at(0) <= L'9');
}

bool qsp_string::IsInClass(char charClass)
{
    *begin() < 128 && ((qspAsciiClasses[*begin()] & charClass) != 0)
}

bool qsp_string::IsAnyInClass(char charClass)
{
    for (auto it : *this) if ((it) < 128 && ((qspAsciiClasses[it] & charClass) != 0)) return true;
    return false;
}

String qsp_string::GetString()
{//TODO: will be it better to use method "find"? i'm shurem its possible and better than just iterate one-by-one.
 //at first, it is quote as the beginning symbol. we can search similar symbol from begin+1, if it is not only one symbol. anyway, if quote is only one, "find" will return end().
//of course, using "find" little more complex by the first sight, but better and more transparent after realisation
    String result;
    result.clear();
    SIterator pos = begin(), quot = begin();

    while (1)
    {
        if (++pos >= end())
        {
            SetError(QSP_ERR_QUOTNOTFOUND);
            result.clear();
            return result;
        }
        if (*pos == *quot && (++pos >= end() || *pos != *quot)) break;
        result += *pos;
    }
    assign(substr(distance(pos, quot)));
    return result;
}

String qsp_string::GetName()
{//��� ����� ���� � ������������ find �� - ��� ��������� �������� ���������� ��� �����.
    SIterator endPos = begin();
    while (++endPos < end())//move iterator to next, until end of string
    {
        if (*endPos < 128 && ((qspAsciiClasses[*endPos] & QSP_CHAR_DELIM) != 0))
            break; //if (IsInClass(QSP_CHAR_DELIM)) break
    }
    String result = substr(0, distance(begin(), endPos));
    assign(substr(distance(begin(), endPos)));
    return result;
}

void qsp_string::SkipN(int n)
{
    assign(substr(n));
}

qsp_CompiledExpression qsp_string::CompileExpression()
//TODO: ������� � ������ ����� ������ � ��������� ��� �. ����, � ����� ������ ������� �����������, � ����� ���������.
{
    qsp_CompiledExpression CompExpr; //������ ��� ������ ����������������� ����
    CompExpr.itemsCount = 0;
    qsp_expression Expression; //����
    //String name; //������� �����, ������ ��� ����� ��� ������ ���������� � ��������� ������ � ��������� �� ������ �� ���. ����� ������� �� ����������.
    bool waitForOperator = false; // ��� ��� ����������, ����� ���������� �����, ���������� �� ����� ����� ������ ����
    char opCode;
    int opSp = -1;//������(�������������) �����
    if (!Expression.PushOpCode(&opSp, qspOpStart))return CompExpr; //����� � �������, ��� ������� ++opSp ��������: � ���� ���������� - ��� ���������, � ���� ���������� ����������� ����� ���������� ����� ���������. � ������ ������, ������ ����� � ���� ������� - ��� � ��������� �������.
    //TODO: ����������� � ���������� ��������� - �������� ������ ������� �� ��� ��� ����.
    else {
        while (1)
        {
            this->SkipSpaces();
            if (waitForOperator)
            {
                opCode = this->OperatorOpCode();//pop operator name from String and get its code : �������� ��� ��������� �� ������ � �������� ��� ���
                if (opCode == qspOpUnknown || opCode >= qspOpFirst_Function)
                {// ��������, ��� ���������� ��� ��������� - ����������. ����� ���-����� � �����
                    SetError(QSP_ERR_UNKNOWNACTION);
                    break;
                }
                /* We want to separate keywords : �� ����� ����� ����� ����������� */
                if ((opCode == qspOpAnd || opCode == qspOpOr || opCode == qspOpMod) && (this->empty() || !this->IsInClass(QSP_CHAR_SPACE | QSP_CHAR_QUOT | QSP_CHAR_EXPSTART)))
                {// �������� ��� ����� "�", "���", "���" ������ ���� ����� ���������� �������� ���� �� ������ � ������� ��� ������
                    SetError(QSP_ERR_SYNTAX);
                    break;
                }
                while (MathOperations[opCode]->Priority <= MathOperations[Expression.opStack[opSp]]->Priority && MathOperations[Expression.opStack[opSp]]->Priority != 127)
                { //���, ������. ���� ��� � ��� ���������. ���� ��������� ������, ��� � ���� � �����, � � ���� � ����� �� �� ����� 127 �� �������� ��������� �� �����.
                    qsp_variant v; //TODO: ������ ���� ��� ��������-����������� �������, ��� ���������������� Append, �������� �� ���������.
                    if (!CompExpr.Append(Expression.opStack[opSp], Expression.argStack[opSp], v)) break;
                    // �������� Append � ���� ����������� ��� ���� ����, ������ ���� (���������� �����������), �� ��������� ��� ���������, �������� ��� - ������������� ������ ����� ����� ��������. ����� ���� ��� - �� ��� �� �� ���� � ����� �� ������. ��� ��� ������� ������ ��� �����������
                    // ����������� � ������� ���� �����:
                    // ��� ��� ����� �� ���� ���� - ��� ������ ��������� V, ���� ��� ��� �� ���� ����������������.
                    if (--opSp < 0)
                    {// �����, ����������� ������ ��������� � ����� (� ���� �� ����� �������� ����� ����, ������ ������ � ������ ����� ����� �������� ���������)
                        SetError(QSP_ERR_SYNTAX);
                        break;
                    }//���� ����� ����������, ��� ����� �� ������� - �������� ����������� �, ���� ����, ����������� �����.
                }
                if (ErrorNum) break; //���� ���� ������ � ����� ������� ����� - �� � ������� ���� ������ ���������.
                switch (opCode)
                {// ���� ������ ����� ���������� ������ ��� ������ �� ������������ ������ ���������� - ������� ��� ���������
                    // �����, ���� ��� - ������� �����
                    //������, ������ �������, ������� - �������� �� ��������� � ����������
                    case qspOpEnd:{
                        if (opSp)//���� opSp ����� 1 ��� ��������� ��� - �� ���� ������ � ����� �����
                        {
                            SetError(QSP_ERR_BRACKNOTFOUND);
                            break;
                        } // �����, ���� opSp ���� - �� ��� ������� � ���� ���������� ���������������� ����������
                        return CompExpr; //��� ��� ������� ����� �� ����� �������� - ������ ���, � ���� �� ���������� ���� � ���������
                    }
                    case qspOpCloseBracket: {
                        if (Expression.opStack[opSp] != qspOpOpenBracket)
                        {//���� �������� - ����������� ������, � � ����� ��������� ���������� �� ������ �� ����������� = ������. ���� � ����� �����
                            SetError(QSP_ERR_BRACKNOTFOUND);
                            break;
                        }
                        //�����, ������ ������� ������.
                        opCode = Expression.opStack[--opSp]; //��� � ����������� ���� � ����� � ������ � ���
                        if (opCode >= qspOpFirst_Function && (++Expression.argStack[opSp] < MinArgsCount(opCode) || Expression.argStack[opSp] > MaxArgsCount(opCode))) SetError(QSP_ERR_ARGSCOUNT);//���� ������ �� ���� - ������, �� � ���������� ������� �������, � ��������� ��� �� ������� �� �����. ���� �����, ������ ������.
                        break;//� ���� �� ����� - �������� ����
                    }
                    case qspOpCloseArrBracket: {
                        if (Expression.opStack[opSp] != qspOpOpenArrBracket)
                        {//�������� ����������� ������ �����, ��������� ��� ���������� �������� ���� �����������. ���? - ������, � �����(�����)
                            SetError(QSP_ERR_BRACKNOTFOUND);
                            break;
                        }//�����, ��, ������ ����.
                        opCode = Expression.opStack[--opSp];//�������� �� ����������� ���� � �����
                        if (opCode != qspOpArrItem) 
                        {// �� � ���������, ���� ��� �� ������� ������� - ���� ������ � ����� �����
                            SetError(QSP_ERR_SYNTAX);
                            break;
                        }//����� - ��, ��� ������� ������� - �� ��.
                        Expression.argStack[opSp]++;//� ���� �� �� - ����������� ������� ����������.
                        break;
                    }
                    case qspOpComma:{
                        if (Expression.opStack[opSp] == qspOpOpenBracket && Expression.opStack[opSp - 1] >= qspOpFirst_Function)
                        {//�������� �����, ������� - ���� ����������� ������, � ����� ��� ������
                            if (++(Expression.argStack[opSp - 1]) > MaxArgsCount(Expression.opStack[opSp - 1]))
                            {//�������� ������� ���������� ��������� (������ �������), � ����� �� �������� ����������� ���������� - ���� ������ � ����� �����
                                SetError(QSP_ERR_ARGSCOUNT);
                                break;
                            }
                        }
                        else
                        {//� ���� ������ ��� ������� (��� ���� � �������) �� ���� - ����� �����
                            if (!Expression.PushOpCode(&opSp, qspOpComma)) break;
                        }
                        waitForOperator = false;
                        break;
                    }
                    default: {
                        if (!Expression.PushOpCode(&opSp, opCode))break;
                        waitForOperator = false;
                        break;
                    }
                }
                if (ErrorNum) break; //���� ��������� ������ � ����� - �������� ����
            }
            else
            {// ���� ��������� �� ��� - �� ������, �� ��� ������.
            //� ���������, ��� ���� ����� ��������:
            //������ - ������. ���� ������ �����, �� ������ ��� ��������� ����� �� �������.
            //������ - �������� � ���������� � �����������: 3(������ ��������), 0(������ ���������� ����), ���-�� ������
                //��� ��� ������ ���� "���� ���������"
            //������� - ��������: 4(�������������� �������� = ������), 0, ���������� ������.
                //���� �� ������ ���� "���� ���������"
            //�������� ������ "{" - ��������: 3, 0, ������
                //� ��, ����� ������ ����
            //����� - �� ��������, �� ����� � ���� � ������, � ����� ��� �� ������. �������� - �� ���� ���������.
            //������ "(" - �� ��������, ����� � ����. � �� ���� ���������
            //������ ")" - �� ��������, �� �����, �������� - ������ �������� � ��������� �� ����� ����������� �����. �������� ������+ ����� ��������.
            //�����(������, �����, ������� - ���� ����) - ��� ��� ���� ���������!
                //��-������, ��� ���� ���������� ����� ����-����, ������������ �������.
                    //� ���� ������, �������� ������ ��������� �������, ������ ��� ������, � ����� ��� ������
                    //���� ��, � ����� �������� ��� ���������� ��� +1, � ������� ������� ����������� ������ �����. ���� ������� ���� - ����� ��� ���, � �����, ��������� ��� ����� � ���(������) ������ �� ���� �����
                    //��� ������ - ���� ���������
                //��-������, �� ����-����, ������ �� � ������ ����������.
                    // ����� ��� ���������� �� ����� �������,
                        // ���� ��� = ������+ ��
                            //������� ������. ���� ������ - ����� ��� � ������.
                            //��� ������ - ������� ������� ����������
                            // ������ ���� = ������
                            //������ ���� - ����� ���
                                // ���� - ������� ���������� (�������� - ������ ��������� ��� �� ������!)
                                // ���� - ���� ��������.
                        // ��� ������ ������� - ��������: 3 (�����), 0, ���� (������ ��� �������� = ���-���)
                            //���� ������ ������ - ����� �, � �� ���� ������ ������ (����� ��������� ����)
                                //���� ������ ��������� - ����� �, ����� �����������, ������� ������� ���������� � ���� ���������
                                //���� �� ��������� - �� ��� ������� ������� (���� ���� ������ �� ������ ��������. ������ - ����� �������, ������� ��� ���������� � ����� ��� ������� ������ �������.
                            //���� �� ����������� ������ - �� ����� ������� � ������� ��� ��������
            //���� �������� - ������ ����������.
                if (this->empty())
                {// � ���� ��� ���� ������ �����
                    if (Expression.opStack[opSp] >= qspOpFirst_Function)
                        SetError(QSP_ERR_ARGSCOUNT); //� �������� �������� ���� "������ �������" - �� ������ � ���������� ����������
                    else
                        SetError(QSP_ERR_SYNTAX); //���� ������ ����������
                    break;
                }
                else if (this->IsDigit())// � ���� ������ �� �����, � ��� ��� ��� ������
                {
                    qsp_variant v;
                    v.Type = QSP_TYPE_NUMBER;
                    *(v.Val.Num) = this->GetNumber();
                    if (Expression.opStack[opSp] == qspOpNegation)
                    {
                        *(v.Val.Num) = -*(v.Val.Num);
                        --opSp;
                    }
                    // TODO: ����, � ������������, � ���������� ������ ������ ��� ���� ���������.
                    if (!CompExpr.Append(qspOpValue, 0, v))break;
                    waitForOperator = true;
                }
                else if (this->IsInClass( QSP_CHAR_QUOT))
                {//������ �� �����, � ��� �������, ������ ������ ����-�� ���� ��������� ���������
                    qsp_variant v;
                    v.Type = QSP_TYPE_STRING;
                    v.Val.Str->assign(GetString());
                    if (ErrorNum) break;
                    if (!CompExpr.Append(qspOpValueToFormat,0,v)) break;
                    waitForOperator = true;
                }
                else if (*this->begin() == QSP_LQUOT[0]) // "{"
                {// ������ �� �����, � ��� ����������� �������� ������
                    qsp_variant v;
                    v.Type = QSP_TYPE_CODE;
                    v.Val.Str->assign(this->GetQString());
                    if (ErrorNum) break;
                    //TODO: �� ��������, ��� � ������� ���� ��������/��������� ������ ������ ������ ����� Append. ��������� ��� ��� ��������
                    if (!CompExpr.Append(qspOpValue,0,v))
                    {
                        //qspFreeString(QSP_STR(v)); //�� ����, ������� �������� �������� � ���� ��������� ����� � ������ �� ������� ����-������. ��� ���������� ������� ���������, � � ����������� �������� ����� �������� ��� �� �� ���������. upd! �������� � �������, ��� ������� ������ �� ������� ���� ������ ���� � � ������ ���� ����� �������� �������� ���.
                        break;
                    }
                    waitForOperator = true;
                }
                else if (*this->begin() == QSP_NEGATION[0]) // "-"
                {
                    if (!Expression.PushOpCode(&opSp, qspOpNegation)) break;
                    s.Str += QSP_STATIC_LEN(QSP_NEGATION);
                    //TODO: ����� ������ ��������� ������ ������. �������� ������ ����� "�������� ������ � ����" � ����� ��� �����.
                }
                else if (*this->begin() == QSP_LRBRACK[0])// "("
                {
                    if (!Expression.PushOpCode(&opSp, qspOpOpenBracket)) break;
                    s.Str += QSP_STATIC_LEN(QSP_LRBRACK); //�� ���� - �������� ������
                }
                else if (*this->begin() == QSP_RRBRACK[0]) // ")"
                {// � ��� ��� ���������. �� ������, � ��� ��������� �������� ���� �� �����
                    opCode = Expression.opStack[opSp];
                    if (opCode != qspOpOpenBracket)
                    {// � ���� ���������, ��� ��� �� ����������� ������ - ������ ������. ���� ��� ������+ �� ������ �������� ����������, � ����� - ������ ����������
                        if (opCode >= qspOpFirst_Function)
                            SetError(QSP_ERR_ARGSCOUNT);
                        else
                            SetError(QSP_ERR_SYNTAX);
                        break;
                    }
                    opCode = Expression.opStack[--opSp];// ����, ����������� ������ ����. ����� ��������� ��� ����� ���.
                    if (opCode < qspOpFirst_Function)
                    {// � ���� ��� ���� ���-�� ������ ������� - ������ ����������
                        SetError(QSP_ERR_SYNTAX);
                        break;
                    }
                    // ����, ����� ����������� ������ ��� ������+
                    if (Expression.argStack[opSp] < MinArgsCount(opCode))
                    {//������� ������� ���������� ����� �������+, � ���� ����� ���������� �� �������� �� �������� - ����� � �������.
                        SetError(QSP_ERR_ARGSCOUNT);
                        break;
                    }
                    s.Str += QSP_STATIC_LEN(QSP_RRBRACK);
                    waitForOperator = true;
                    //�� � �������, ��� �������� ���������, ��������� ����� �� ��� �� ���������� �� ������+ ����� ������, � ����� ����������� ����������. ����� �� ������ ����������� ����� � ���� ���������� ���������.
                }
                else if (!this->IsInClass(QSP_CHAR_DELIM))
                {
                    String name = this->GetName();
                    if (ErrorNum) break;
                    this->SkipSpaces();
                    if (*name.begin() == QSP_USERFUNC[0])
                    {
                        qsp_variant v;
                        v.Type = QSP_TYPE_STRING;
                        v.Val.Str->assign(name.substr(1));/* Ignore a @ symbol */
                        if (!CompExpr.Append(qspOpValue,0,v)) break;
                        /* Add a function call */
                        if (!Expression.PushOpCode(&opSp, qspOpFunc)) break;
                        Expression.argStack[opSp]++;
                        if (!this->empty() && *this->begin() == QSP_LRBRACK[0])
                        {
                            if (!Expression.PushOpCode(&opSp, qspOpOpenBracket)) break;
                            s.Str += QSP_STATIC_LEN(QSP_LRBRACK);
                        }
                        else waitForOperator = true;
                    }
                    else
                    {
                        opCode = FunctionOpCode(name);
                        if (opCode >= qspOpFirst_Function)
                        {
                            if (!this->empty() && *this->begin() == QSP_LRBRACK[0])
                            {
                                if (!Expression.PushOpCode(&opSp, opCode)) break;
                                if (!Expression.PushOpCode(&opSp, qspOpOpenBracket)) break;
                                s.Str += QSP_STATIC_LEN(QSP_LRBRACK);
                            }
                            else if (MinArgsCount(opCode) < 2)
                            {
                                if (!Expression.PushOpCode(&opSp, opCode)) break;
                                if (MinArgsCount(opCode))
                                {
                                    /* The function has a single argument */
                                    Expression.argStack[opSp]++;
                                }
                                else
                                {
                                    /* The function has no arguments */
                                    waitForOperator = true;
                                }
                            }
                            else
                            {
                                SetError(QSP_ERR_BRACKSNOTFOUND);
                                break;
                            }
                        }
                        else
                        {
                            qsp_variant v;
                            v.Type = QSP_TYPE_VARREF;
                            v.Val.Str->assign(name);
                            if (!CompExpr.Append(qspOpValue,0,v)) break;
                            if (!this->empty() && *this->begin() == QSP_LSBRACK[0])
                            {
                                s.Str += QSP_STATIC_LEN(QSP_LSBRACK);
                                this->SkipSpaces();
                                if (!this->empty() && *this->begin() == QSP_RSBRACK[0])
                                {
                                    s.Str += QSP_STATIC_LEN(QSP_RSBRACK);
                                    if (!Expression.PushOpCode(&opSp, qspOpLastArrItem)) break;
                                    Expression.argStack[opSp]++;
                                    waitForOperator = true;
                                }
                                else
                                {
                                    if (!Expression.PushOpCode(&opSp, qspOpArrItem)) break;
                                    Expression.argStack[opSp]++;
                                    if (!Expression.PushOpCode(&opSp, qspOpOpenArrBracket)) break;
                                }
                            }
                            else
                            {
                                if (!Expression.PushOpCode(&opSp, qspOpArrItem)) break;
                                Expression.argStack[opSp]++;
                                waitForOperator = true;
                            }
                        }
                    }
                }
                else
                {
                    SetError(QSP_ERR_SYNTAX);
                    break;
                }
            }
        }
        //� ��� ���� ���� ������� ������ �� ������� ���������. � ������ ��� ���� ����������� ��� �����.
        //TODO: ������� ��� ���� ������� �������.
        while (--(CompExpr.itemsCount) >= 0)
        {//� ��������, ��� ���������� � �����: �� �������, ������������ �� ������� � ���� �� ����� ����, ��� ������� ��������� ��������� �����, �������: ���� ��� �������� �����, ���� �����-��-������, �� ��� ����� �����, ��� ����� ��������� - ������ �� ���. � ����� ������, �� ������ ����������.
        //������: �� �����-����� ��� ������ ������ (������!) �����, ������� ���� ����� ��������� ������������. ������� �������� � �������� ������ �����������. �� � ��, � ������ ��������� �� �������� ������ ������ ����� ��� - ���� ��-�����, ���� ��-�����-��-������.
        //������, ��� � ��������, ������� ������. � ���� ���� ����������� ����� ����� ����. ������ ��� ����� ���� � ������ ���� �����������. ������, ���������� ����. ������ ������� - ��������������� �������� ���� ��������.
        // � ��������������� ��� ����� ���� ������ ���� ���� ���� �� ������ ��� �� ��������, � ������� ������� ��������� �������� ���-�� ����. � ����� �� ������� ���������� �� � ����� ���� ������� ��������, � �������� �������� ��������� ���-�� ������, � ����. � ������ �����, ������.
            switch (CompExpr.compOpCodes[CompExpr.itemsCount])
            {
            case qspOpValue:
            case qspOpValueToFormat:
                if (QSP_ISSTR(CompExpr.compValues[CompExpr.itemsCount].Type)) qspFreeString(QSP_STR(CompExpr.compValues[CompExpr.itemsCount]));
                break;
            }
        }
    }
    CompExpr.itemsCount = 0;
    return CompExpr;
}

char qsp_string::OperatorOpCode()
{ 
    int i;
    qsp_MathOpNames::iterator name;
    if (this->empty()) return qspOpEnd;// ���������� ��� ���������. ����� ����� ������� ��(����) � ��������� ������?
    for (i = 0; i < QSP_OPSLEVELS; ++i)
    {
        name = MathOpNames[i].GetCodeByName(*this);
        if (name != MathOpNames[i].end())
        {
            this->assign(substr((*name)->Name.length()));
            return (*name)->Code;
        }
    }
    return qspOpUnknown;
}

String& qsp_string::operator=(const String& source)
{
    this->assign(source);
    return *this;
};


SIterator qsp_string::DelimPos(wchar_t ch) //if not found, returns end-iterator. any found symbol can be only pre-end, because end is empty pointer
{
    int c1 = 0; // counter for Round Brackets
    int c2 = 0; // counter for Square Brackets
    int c3 = 0; //counter for Figure Brackets 
    SIterator pos = begin();
    wchar_t quot;
    while (pos < end())
    {
        if (*pos < 128 && ((qspAsciiClasses[*pos] & 2) != 0)) // QSP_CHAR_QUOT = 2
        { //here we skip "underquoted space"
            quot = *pos;
            while (++pos < end())
                if (*pos == quot && (++pos >= end() || *pos != quot)) break;
            if (pos >= end()) return end();// was: return 0 - in this case
        }
        if (*pos == L'(') ++c1;
        else if (*pos == L')'){if (c1) --c1;}
        else if (*pos == L'[') ++c2;
        else if (*pos == L']'){if (c2) --c2;}
        else if (*pos == L'{')++c3;
        else if (*pos == L'}'){if (c3) --c3;
        if (!c1 && !c2 && !c3 && *pos == ch) return pos;
        ++pos;
    }
    return end (); //was: return 0  - in this case
};