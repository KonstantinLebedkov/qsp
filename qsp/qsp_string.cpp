#include <cwctype>
#include "qsp_string.h"
#include "qsp_variants.h"
#include "qsp_MathOpName.h"

bool qsp_string::replaceEntryText(const qsp_string from, const qsp_string to) {
    size_t start_pos = this->find(from);
    if (start_pos == qsp_string::npos)
        return false;
    this->replace(start_pos, from.length(), to);
    return true;
};

bool qsp_string::replaceEntryText(const std::wstring from, const std::wstring to) {
    size_t start_pos = this->find(from);
    if (start_pos == qsp_string::npos)
        return false;
    this->replace(start_pos, from.length(), to);
    return true;
};

void qsp_string::SkipSpaces()
{
    while ((*begin() == *(L" ")) && begin() != end()) this->erase(0, 1);
};

qsp_string qsp_string::replaceText(const qsp_string from, const qsp_string to) {
    qsp_string result = qsp_string(*this);
    while (result.replaceEntryText(from, to)) {};
    return result;
};

qsp_string qsp_string::replaceText(const std::wstring from, const std::wstring to) {
    qsp_string result = qsp_string(*this);
    while (result.replaceEntryText(from, to)) {};
    return result;
};

void qsp_string::replaceTextInSelf(const qsp_string from, const qsp_string to) {
    while (this->replaceEntryText(from,to)){}
};

void qsp_string::replaceTextInSelf(const std::wstring from, const std::wstring to) {
    while (this->replaceEntryText(from, to)) {}
};

bool qsp_string::IsAnyString()
{
    //TODO: IMHO, it is not good to check for spaces here. it must be cleared from spaces during initial parsing and nomore else.
    //TODO: this method called for some variant fields. for example, in qsp.actions->AddAction. for union, it must be checked for field type before, another case may cause error.
    qsp_string str(*this);
    str.SkipSpaces();
    return str.empty();
}

int qsp_string::ToInteger()
{
    return std::stoi(*this);
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
    qsp_string::iterator begin = this->begin();
    qsp_string::iterator end = this->end();
    while (begin < end && *begin != L' ') ++begin;
    while (begin < end && *(end - 1)!=L' ') --end;
    this->assign(this->substr(distance(this->begin(), begin), distance(begin, end)));
}

void qsp_string::ExprValue()
{
    qsp_variants compValues; //объявляет массив вариантов, размерностью 200. нахуа?
    std::vector<char> compOpCodes[QSP_MAXITEMS], compArgsCounts[QSP_MAXITEMS]; // объявляет 2 массива чаров, размерностью 200 каждый.
    int itemsCount;
    if (!(itemsCount = qspCompileExpression(expr, compValues, compOpCodes, compArgsCounts)))
        //вызвает "скомпилировать объявление", дав тому строку и 3 вышеобъявленных массива. и получить в ответ итемс-каунт
        return qspGetEmptyVariant(QSP_TYPE_UNDEFINED); //если итемс-каунт оказался нулем, тоесть фолс - инвертируем в тру и из функции выдаем эмпти-вариант, с типом "неопределен"
    return qspValue(--itemsCount, compValues, compOpCodes, compArgsCounts); /* the last item represents the whole expression */
}

int qsp_string::CompileExpression()
//INLINE int qspCompileExpression(QSPString s, QSPVariant *compValues, QSP_TINYINT *compOpCodes, QSP_TINYINT *compArgsCounts)
{
    // а дальше вопрос: не подвключая "кусп-варианты" в хедер, что сделает его закольцованным и оттого некомпилируемым, не получится вписать в методы функции, перекидывающиеся между собой векторами. и вот тут уже вопрос: нахуа "компайл-экспрессион" вынесена в отдельную функцию? она статична и широко-видима? она в своем хедере изначально? или она локальна и только внутри Срр-файла? ответ - локальна. а потому - наххуа?!
    // более того, поиск показал, что она вообще нигде более не используется. и она не под циклом. а потому её НЕ БУДЕТ как отдельной функции. ибо в том нет нималейшего смысла. но переработать её все же надо
    qsp_variants compValues; //объявляет массив вариантов, размерностью 200. нахуа?
    std::vector<char> compOpCodes, compArgsCounts;
    //выше переобъявляем переданное. не копируем в тело основной функции по итогам рабора.
    qsp_variant v;
    qsp_string name;
    bool waitForOperator = false;
    char opCode;
    //char opStack[QSP_STACKSIZE], argStack[QSP_STACKSIZE]; //stack-size = 30
    std::vector<char> opStack, argStack;
    int itemsCount = 0, opSp = -1;
    if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpStart)) return 0; //qspOpStart - member of enum declared in mathops.h
    // local declared bool func qspCompileExprPushOpCode
    // возвращает false только в случае stackOverflow, имхо, задействовать её здесь немного абсурдно, хотя есть смысл в том что работа с массивом, а не вектором, осмысленна - массив ложится в стек.
    else {
        while (1)
        {
            SkipSpaces();//qspSkipSpaces(&s);
            if (waitForOperator)
            {
                opCode = qspOperatorOpCode(&s);
                if (opCode == qspOpUnknown || opCode >= qspOpFirst_Function)
                {
                    qspSetError(QSP_ERR_UNKNOWNACTION);
                    break;
                }
                /* We want to separate keywords */
                if ((opCode == qspOpAnd || opCode == qspOpOr || opCode == qspOpMod) && (qspIsEmpty(s) || !qspIsInClass(*s.Str, QSP_CHAR_SPACE | QSP_CHAR_QUOT | QSP_CHAR_EXPSTART)))
                {
                    qspSetError(QSP_ERR_SYNTAX);
                    break;
                }
                while (qspOps[opCode].Priority <= qspOps[opStack[opSp]].Priority && qspOps[opStack[opSp]].Priority != 127)
                {
                    if (!qspAppendToCompiled(opStack[opSp], &itemsCount, compValues, compOpCodes, compArgsCounts, argStack[opSp], v)) break;
                    if (--opSp < 0)
                    {
                        qspSetError(QSP_ERR_SYNTAX);
                        break;
                    }
                }
                if (qspErrorNum) break;
                switch (opCode)
                {
                case qspOpEnd:
                    if (opSp)
                    {
                        qspSetError(QSP_ERR_BRACKNOTFOUND);
                        break;
                    }
                    return itemsCount;
                case qspOpCloseBracket:
                    if (opStack[opSp] != qspOpOpenBracket)
                    {
                        qspSetError(QSP_ERR_BRACKNOTFOUND);
                        break;
                    }
                    opCode = opStack[--opSp];
                    if (opCode >= qspOpFirst_Function)
                    {
                        ++argStack[opSp];
                        if (argStack[opSp] < qspOps[opCode].MinArgsCount || argStack[opSp] > qspOps[opCode].MaxArgsCount)
                            qspSetError(QSP_ERR_ARGSCOUNT);
                    }
                    break;
                case qspOpCloseArrBracket:
                    if (opStack[opSp] != qspOpOpenArrBracket)
                    {
                        qspSetError(QSP_ERR_BRACKNOTFOUND);
                        break;
                    }
                    opCode = opStack[--opSp];
                    if (opCode != qspOpArrItem)
                    {
                        qspSetError(QSP_ERR_SYNTAX);
                        break;
                    }
                    ++argStack[opSp];
                    break;
                case qspOpComma:
                    if (opStack[opSp] == qspOpOpenBracket && opStack[opSp - 1] >= qspOpFirst_Function)
                    {
                        if (++argStack[opSp - 1] > qspOps[opStack[opSp - 1]].MaxArgsCount)
                        {
                            qspSetError(QSP_ERR_ARGSCOUNT);
                            break;
                        }
                    }
                    else
                    {
                        if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpComma)) break;
                    }
                    waitForOperator = QSP_FALSE;
                    break;
                default:
                    if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, opCode)) break;
                    waitForOperator = QSP_FALSE;
                    break;
                }
                if (qspErrorNum) break;
            }
            else
            {
                if (qspIsEmpty(s))
                {
                    if (opStack[opSp] >= qspOpFirst_Function)
                        qspSetError(QSP_ERR_ARGSCOUNT);
                    else
                        qspSetError(QSP_ERR_SYNTAX);
                    break;
                }
                else if (qspIsDigit(*s.Str))
                {
                    v.Type = QSP_TYPE_NUMBER;
                    QSP_NUM(v) = qspGetNumber(&s);
                    if (opStack[opSp] == qspOpNegation)
                    {
                        QSP_NUM(v) = -QSP_NUM(v);
                        --opSp;
                    }
                    if (!qspAppendToCompiled(qspOpValue, &itemsCount, compValues, compOpCodes, compArgsCounts, 0, v)) break;
                    waitForOperator = QSP_TRUE;
                }
                else if (qspIsInClass(*s.Str, QSP_CHAR_QUOT))
                {
                    name = qspGetString(&s);
                    if (qspErrorNum) break;
                    v.Type = QSP_TYPE_STRING;
                    QSP_STR(v) = name;
                    if (!qspAppendToCompiled(qspOpValueToFormat, &itemsCount, compValues, compOpCodes, compArgsCounts, 0, v))
                    {
                        qspFreeString(QSP_STR(v));
                        break;
                    }
                    waitForOperator = QSP_TRUE;
                }
                else if (*s.Str == QSP_LQUOT[0])
                {
                    name = qspGetQString(&s);
                    if (qspErrorNum) break;
                    v.Type = QSP_TYPE_CODE;
                    QSP_STR(v) = qspGetNewText(name);
                    if (!qspAppendToCompiled(qspOpValue, &itemsCount, compValues, compOpCodes, compArgsCounts, 0, v))
                    {
                        qspFreeString(QSP_STR(v));
                        break;
                    }
                    waitForOperator = QSP_TRUE;
                }
                else if (*s.Str == QSP_NEGATION[0])
                {
                    if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpNegation)) break;
                    s.Str += QSP_STATIC_LEN(QSP_NEGATION);
                }
                else if (*s.Str == QSP_LRBRACK[0])
                {
                    if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpOpenBracket)) break;
                    s.Str += QSP_STATIC_LEN(QSP_LRBRACK);
                }
                else if (*s.Str == QSP_RRBRACK[0])
                {
                    opCode = opStack[opSp];
                    if (opCode != qspOpOpenBracket)
                    {
                        if (opCode >= qspOpFirst_Function)
                            qspSetError(QSP_ERR_ARGSCOUNT);
                        else
                            qspSetError(QSP_ERR_SYNTAX);
                        break;
                    }
                    opCode = opStack[--opSp];
                    if (opCode < qspOpFirst_Function)
                    {
                        qspSetError(QSP_ERR_SYNTAX);
                        break;
                    }
                    if (argStack[opSp] < qspOps[opCode].MinArgsCount)
                    {
                        qspSetError(QSP_ERR_ARGSCOUNT);
                        break;
                    }
                    s.Str += QSP_STATIC_LEN(QSP_RRBRACK);
                    waitForOperator = QSP_TRUE;
                }
                else if (!qspIsInClass(*s.Str, QSP_CHAR_DELIM))
                {
                    name = qspGetName(&s);
                    if (qspErrorNum) break;
                    qspSkipSpaces(&s);
                    if (*name.Str == QSP_USERFUNC[0])
                    {
                        /* Ignore a @ symbol */
                        name.Str += QSP_STATIC_LEN(QSP_USERFUNC);
                        /* Add the loc name */
                        v.Type = QSP_TYPE_STRING;
                        QSP_STR(v) = qspGetNewText(name);
                        if (!qspAppendToCompiled(qspOpValue, &itemsCount, compValues, compOpCodes, compArgsCounts, 0, v))
                        {
                            qspFreeString(QSP_STR(v));
                            break;
                        }
                        /* Add a function call */
                        if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpFunc)) break;
                        ++argStack[opSp];
                        if (!qspIsEmpty(s) && *s.Str == QSP_LRBRACK[0])
                        {
                            if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpOpenBracket)) break;
                            s.Str += QSP_STATIC_LEN(QSP_LRBRACK);
                        }
                        else
                        {
                            waitForOperator = QSP_TRUE;
                        }
                    }
                    else
                    {
                        opCode = qspFunctionOpCode(name);
                        if (opCode >= qspOpFirst_Function)
                        {
                            if (!qspIsEmpty(s) && *s.Str == QSP_LRBRACK[0])
                            {
                                if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, opCode)) break;
                                if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpOpenBracket)) break;
                                s.Str += QSP_STATIC_LEN(QSP_LRBRACK);
                            }
                            else if (qspOps[opCode].MinArgsCount < 2)
                            {
                                if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, opCode)) break;
                                if (qspOps[opCode].MinArgsCount)
                                {
                                    /* The function has a single argument */
                                    ++argStack[opSp];
                                }
                                else
                                {
                                    /* The function has no arguments */
                                    waitForOperator = QSP_TRUE;
                                }
                            }
                            else
                            {
                                qspSetError(QSP_ERR_BRACKSNOTFOUND);
                                break;
                            }
                        }
                        else
                        {
                            v.Type = QSP_TYPE_VARREF;
                            QSP_STR(v) = qspGetNewText(name);
                            if (!qspAppendToCompiled(qspOpValue, &itemsCount, compValues, compOpCodes, compArgsCounts, 0, v))
                            {
                                qspFreeString(QSP_STR(v));
                                break;
                            }
                            if (!qspIsEmpty(s) && *s.Str == QSP_LSBRACK[0])
                            {
                                s.Str += QSP_STATIC_LEN(QSP_LSBRACK);
                                qspSkipSpaces(&s);
                                if (!qspIsEmpty(s) && *s.Str == QSP_RSBRACK[0])
                                {
                                    s.Str += QSP_STATIC_LEN(QSP_RSBRACK);
                                    if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpLastArrItem)) break;
                                    ++argStack[opSp];
                                    waitForOperator = QSP_TRUE;
                                }
                                else
                                {
                                    if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpArrItem)) break;
                                    ++argStack[opSp];
                                    if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpOpenArrBracket)) break;
                                }
                            }
                            else
                            {
                                if (!qspCompileExprPushOpCode(opStack, argStack, &opSp, qspOpArrItem)) break;
                                ++argStack[opSp];
                                waitForOperator = QSP_TRUE;
                            }
                        }
                    }
                }
                else
                {
                    qspSetError(QSP_ERR_SYNTAX);
                    break;
                }
            }
        }
        while (--itemsCount >= 0)
        {
            switch (compOpCodes[itemsCount])
            {
            case qspOpValue:
            case qspOpValueToFormat:
                if (QSP_ISSTR(compValues[itemsCount].Type)) qspFreeString(QSP_STR(compValues[itemsCount]));
                break;
            }
        }
    }
    return 0;
}

char qsp_string::OperatorOpCode()
{ //в сущности делает что: ищет среди операторов такой, какой начинает строку. и если находит, то: кидает в результ код оператора, и вырезает из строки его текст.
    // а мы будем делать вот что: звать поиск как метод синглтон итератора. и ежели поиск удался, отразать кусок строки в начале, возвращая код(или итератор) оператора
    int i;
    qsp_MathOpName* name;
    if (this->empty()) return qspOpEnd;// возвращает код оператора. имеет смысл заявить их в отдельном хедере?
    for (i = 0; i < QSP_OPSLEVELS; ++i)
    {
        name = (qsp_MathOpName*)bsearch(this, qspOpsNames[i], qspOpsNamesCounts[i], sizeof(QSPMathOpName), qspMathOpStringCompare);
        if (name)
        {
            expr->Str += qspStrLen(name->Name);
            return name->Code;
        }
    }
    return qspOpUnknown;
}

qsp_string& qsp_string::operator=(const std::wstring& source)
{
    this->assign(source);
    return *this;
};