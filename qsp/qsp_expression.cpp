#include "qsp_expression.h"
#include "qsp_errors.h"
#include "qsp_MathOps.h"

bool qsp_expression::PushOpCode(int* opSp, char opCode)
{
    if ((++(*opSp)) == QSP_STACKSIZE)
    {
        SetError(QSP_ERR_STACKOVERFLOW);
        return false;
    }
    opStack[*opSp] = opCode;
    argStack[*opSp] = (opCode < qspOpFirst_Function ? MinArgsCount(opCode) : 0);
    return true;
}

bool qsp_CompiledExpression::Append(char opCode, char argsCount, qsp_variant v)
{
    if (itemsCount == QSP_MAXITEMS)
    {
        SetError(QSP_ERR_TOOMANYITEMS);
        return false;
    }
    compOpCodes[itemsCount] = opCode;
    compArgsCounts[itemsCount] = argsCount;
    if (opCode== qspOpValue ||opCode == qspOpValueToFormat)compValues[itemsCount] = v;
    ++itemsCount;
    //TODO: по моему разумению, необходимо экспрессьон заявлять как экспрессьонС - вектор отдельных экспрессьон, содержащих: код, число аргументов, вариативную переменную. это будет сильно так проще.
    return true;
}

qsp_variant* qsp_CompiledExpression::Value(int valueIndex) //based on "QSPVariant qspValue(.....)" from mathops.cpp
{
    qsp_variants args; //вектор аргументов
    qsp_variant *tos; //выводная вариативная переменная
    int i;
    int oldRefreshCount; //фрейм-рейт, защита от фризов?!
    int argIndices[QSP_OPMAXARGS]; //массив индексов аргументов. нуу... может быть, здесь лучше бы смотрелся массив пойнтеров на элементы вектора?
    String name;
    char type;
    char opCode; //код операции
    char argsCount; //счетчик аргументов операции
    if (valueIndex < 0)
    {
        SetError(QSP_ERR_SYNTAX);
        return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
    }
    oldRefreshCount = qspRefreshCount;//ХЗ чё за херь, но походу глобальная. пофиг пока, потом смотреть буду. часть locations.h
    opCode = compOpCodes[valueIndex];
        //берем код операции из массива. (массив набит, набивка в процедуре "CompileExpression"
    argsCount = compArgsCounts[valueIndex];
        //берем количество аргументов операции из массива.
    if (argsCount)
    {
        /* Find positions of arguments : Найдем позиции аргументов */
        --valueIndex; 
        /* move to the last argument : Сдвигаем номер на последний аргумент (видимо, он был на последнем+1, тоесть, "end" */
        for (i = argsCount - 1; i >= 0; --i)  { //набиваем массив аргументами, проходя с конца (на самом деле с указанной точки) массива
            argIndices[i] = valueIndex; //ну да, в список индексов, начиная с последнего, заносим номера, где аргументы сныканы.
            valueIndex = SkipValue(valueIndex);//и скипаем. занесенный, а если он состоит из нескольких, то и его части.
        }
        /* набиваем массив индексов номерами нужных аргументов. внесенные аргументы скипаем, и их составные части, если внесенные аргументы составные */
        switch (opCode)
        {
            case qspOpAnd:
            case qspOpOr:
            case qspOpIIf:{
                /* We don't pre-evaluate arguments */
                break;
            }
            default: {
                for (i = 0; i < argsCount; ++i)
                {
                    args.push_back(ArgumentValue(argIndices[i], MathOperations[opCode]->ArgsTypes[i])); 
                    //расчитывается аргумент с заданным номером, и представленный в типе варианта вносится в вектор "вариантс"
                    //тоесть, это должно звучать как "аргс->пуш_бек(АргументВэлюэ)", где аргумент-веэлюе есть метод класса компайледЭкспрессьон, коий выдает результатом переменную типа "вариант", а в качестве аргумента (комп-вэлюес, комп-кодес и комп-аргс-каунтс не нужны, ибо собственные поля) получает значение из индекса аргументов, тоесть индекс, и тип аргумента полученный по коду обрабатываемой операции и типу.
                    //блин, что-то я путаюсь.
                    //разберемся с индексами.
                    //итак, они хранятся в массиве "argIndices" - ноль.
                    //массив "argIndices[QSP_OPMAXARGS]" создан, размер заявлен - раз.
                    //количество аргументов "argsCount" взято как "compArgsCounts[valueIndex]" - два.
                    //если оно больше нуля - заходим в блок набивки массива - три.
                    //в цикле по "i" от argsCount-1 до 0 в массив "argIndices" по адресу "i" кладем (кто бы мог подумать!) ValueIndex, и скипаем составные части.
                    // ну и наконец, мутно-непонятное место, если код операции - не "И", "ИЛИ", "ИИФ", то
                    //в цикле по "i" от 0 до "argsCount-1" включительно в аргс пушим аргумент, посчитанный "ArgumentValue", коему даны: вэлюе-индекс искомого аргумента и тип его полученный, по номеру i
                    if (qspRefreshCount != oldRefreshCount || ErrorNum) //вторая часть ясна - не было ль ошибки. а вот первая? проверка на тормоза?
                    {
                        /* We have to cleanup collected arguments */
                        qspFreeVariants(args, i);
                        /* We have to cleanup other argument sub-trees */
                        while (++i < argsCount)
                            qspFreeValue(argIndices[i], compValues, compOpCodes, compArgsCounts);
                        return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
                    }
                }
                break;
            }
        }
        /*и нафига здесь свич? в случаях "И", "ИЛИ, "ИИФ" он не делает ничего. в иных случаях*/
    }
        /*если в массиве сказано, что для данной операции более нуля аргументов, то выписываем в массив индексов их номера*/
    type = MathOperations[opCode]->ResType;
        //тайп запомнит, какого типа должен получиться результат
    if (QSP_ISDEF(type)) tos->Type = type;
        //если тайп в списке понятных типов результата, вносим его тип в выводную вариативную переменную.
    switch (opCode)
    {
        case qspOpValue: /*just value*/ {
            tos = new qsp_variant(compValues[valueIndex]);
            break;
        }
        case qspOpValueToFormat: /*is not verified yet - format text of value*/ {
            tos = new qsp_variant(compValues[valueIndex]);
            if (tos->IsStr())
            {
                String name = *tos->Val.Str;
                *tos->Val.Str = qspFormatText(name, QSP_TRUE);
                if (qspRefreshCount != oldRefreshCount || ErrorNum) break;
            }
            break;
        }
        case qspOpArrItem:
        case qspOpLastArrItem: /*is not verified yet*/ {
            QSPVar* var;
            int arrIndex;
            name = *args[0]->Val.Str;
            var = qspVarReference(name, QSP_FALSE);
            if (!var) break;
            if (opCode == qspOpLastArrItem)
                arrIndex = var->ValsCount - 1;
            else if (argsCount == 2)
                arrIndex = QSP_ISSTR(args[1].Type) ? qspGetVarTextIndex(var, QSP_STR(args[1]), QSP_FALSE) : QSP_NUM(args[1]);
            else
                arrIndex = 0;
            qspGetVarValueByReference(var, arrIndex, QSP_VARBASETYPE(name), &tos);
            break;
        }
        case qspOpAnd: /* logical AND operator */ {
            args.push_back(ArgumentValue(argIndices[0], QSP_TYPE_NUMBER));
            //тут важно, что пушить нужно в нулевую позицию. а поскольку набивка для "И" не делалась, то вектор пока пуст
            if (qspRefreshCount != oldRefreshCount || ErrorNum)
            {
                qspFreeValue(argIndices[1], compValues, compOpCodes, compArgsCounts);
                return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
            }
            if (QSP_ISTRUE(QSP_NUM(args[0])))
            {
                args.push_back(ArgumentValue(argIndices[1], QSP_TYPE_NUMBER));
                if (qspRefreshCount != oldRefreshCount || ErrorNum)
                    return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
                tos->Val.Num = QSP_TOBOOL(QSP_NUM(args[1]));
            }
            else
            {
                qspFreeValue(argIndices[1], compValues, compOpCodes, compArgsCounts);
                tos->Val.Num = QSP_TOBOOL(QSP_FALSE);
            }
            return tos;
        }
        case qspOpOr: /* logical OR operator */ {
            args.push_back(ArgumentValue(argIndices[0], QSP_TYPE_NUMBER));
            if (qspRefreshCount != oldRefreshCount || ErrorNum)
            {
                qspFreeValue(argIndices[1], compValues, compOpCodes, compArgsCounts);
                return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
            }
            if (QSP_ISTRUE(args[0]->Val.Num))
            {
                qspFreeValue(argIndices[1], compValues, compOpCodes, compArgsCounts);
                tos->Val.Num = QSP_TOBOOL(QSP_TRUE);
            }
            else
            {
                args.push_back(ArgumentValue(argIndices[1], QSP_TYPE_NUMBER));
                if (qspRefreshCount != oldRefreshCount || ErrorNum)
                    return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
                tos->Val.Num = QSP_TOBOOL(QSP_NUM(args[1]));
            }
            return tos;
        }
        case qspOpNot: /* logical NOT operator */ {
            tos.Val.Num = QSP_TOBOOL(!QSP_NUM(args[0]));
            break;
        }
        case qspOpIIf: {
            args.push_back(ArgumentValue(argIndices[0], QSP_TYPE_NUMBER));
            if (qspRefreshCount != oldRefreshCount || ErrorNum)
            {
                qspFreeValue(argIndices[1], compValues, compOpCodes, compArgsCounts);
                qspFreeValue(argIndices[2], compValues, compOpCodes, compArgsCounts);
                return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
            }
            if (QSP_ISTRUE(QSP_NUM(args[0])))
            {
                tos = ArgumentValue(argIndices[1], QSP_TYPE_UNDEFINED);
                //внимание! вопросы: разыменуем мы указатель. присвоим целевой вариант разыменованному.
                //первый - пройдет ли тут нормально присваивание?
                //второй - а что потом станет с объектом под пойнтером? утечет памятью?
                qspFreeValue(argIndices[2], compValues, compOpCodes, compArgsCounts);
            }
            else
            {
                tos = ArgumentValue(argIndices[2], QSP_TYPE_UNDEFINED);
                qspFreeValue(argIndices[1], compValues, compOpCodes, compArgsCounts);
            }
            if (qspRefreshCount != oldRefreshCount || ErrorNum)
                return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
            return tos;
        }
        case qspOpNegation: /*verified*/ {
            *tos->Val.Num = -(*args[0]->Val.Num);
            break;
        }
        case qspOpMul: /*verified*/ {
            *tos->Val.Num = ( * args[0]->Val.Num) * (*args[1]->Val.Num);
            break;
        }
        case qspOpDiv: /*verified*/ {
            if (*args[1]->Val.Num == 0)
            {
                SetError(QSP_ERR_DIVBYZERO);
                break;
            }
            *tos->Val.Num = (*args[0]->Val.Num) / (*args[1]->Val.Num);
            break;
        }
        case qspOpAdd: /*isnt fully converted. there last 2 variants*/ {
            if (args[0]->IsNum() && args[1]->IsNum()) /* tiny optimization for numbers */
            {
                *tos->Val.Num = ( * args[0]->Val.Num) + ( * args[1]->Val.Num);
                tos->Type = QSP_TYPE_NUMBER;
            }
            else if (args[0]->IsStr() && args[1]->IsStr())
            {
                tos->Type = QSP_TYPE_STRING;
                tos->Val.Str->assign(*args[0]->Val.Str);
                tos->Val.Str->append(*args[1]->Val.Str);
            }
            else if (qspIsCanConvertToNum(args) && qspIsCanConvertToNum(args + 1))
            {//isCanConvertToNum - must be method of variant
                args[0]->ConvertTo(QSP_TYPE_NUMBER);
                args[1]->ConvertTo(QSP_TYPE_NUMBER);
                *tos->Val.Num = (*args[0]->Val.Num) + (*args[1]->Val.Num);
                tos->Type = QSP_TYPE_NUMBER;
            }
            else
            {
                /* Result is a string that can't be converted to a number */
                qspConvertVariantTo(args, QSP_TYPE_STRING);
                qspConvertVariantTo(args + 1, QSP_TYPE_STRING);
                qspAddText(&tos.Val.Str, QSP_STR(args[0]), QSP_TRUE);
                qspAddText(&tos.Val.Str, QSP_STR(args[1]), QSP_FALSE);
                tos.Type = QSP_TYPE_STRING;
            }
            break;
        }
        case qspOpSub: {
            tos.Val.Num = QSP_NUM(args[0]) - QSP_NUM(args[1]);
            break;
        }
        case qspOpMod: {
            if (QSP_NUM(args[1]) == 0)
            {
                SetError(QSP_ERR_DIVBYZERO);
                break;
            }
            tos.Val.Num = QSP_NUM(args[0]) % QSP_NUM(args[1]);
            break;
        }
        case qspOpAppend: {
            qspAddText(&tos.Val.Str, QSP_STR(args[0]), QSP_TRUE);
            qspAddText(&tos.Val.Str, QSP_STR(args[1]), QSP_FALSE);
            break;
        }
        case qspOpComma: {
            qspAddText(&tos.Val.Str, QSP_STR(args[0]), QSP_TRUE);
            qspAddText(&tos.Val.Str, QSP_STATIC_STR(QSP_VALSDELIM), QSP_FALSE);
            qspAddText(&tos.Val.Str, QSP_STR(args[1]), QSP_FALSE);
            break;
        }
        case qspOpEq: {
            tos.Val.Num = QSP_TOBOOL(qspAutoConvertCompare(args, args + 1) == 0);
            break;
        }
        case qspOpLt: {
            tos.Val.Num = QSP_TOBOOL(qspAutoConvertCompare(args, args + 1) < 0);
            break;
        }
        case qspOpGt: {
            tos.Val.Num = QSP_TOBOOL(qspAutoConvertCompare(args, args + 1) > 0);
            break;
        }
        case qspOpLeq: {
            tos.Val.Num = QSP_TOBOOL(qspAutoConvertCompare(args, args + 1) <= 0);
            break;
        }
        case qspOpGeq: {
            tos.Val.Num = QSP_TOBOOL(qspAutoConvertCompare(args, args + 1) >= 0);
            break;
        }
        case qspOpNe: {
            tos.Val.Num = QSP_TOBOOL(qspAutoConvertCompare(args, args + 1) != 0);
            break;
        }
                    /* Embedded functions -------------------------------------------------------------- */
        case qspOpLoc: {
            tos.Val.Num = QSP_TOBOOL(qspLocIndex(QSP_STR(args[0])) >= 0);
            break;
        }
        case qspOpObj: {
            tos.Val.Num = QSP_TOBOOL(qspObjIndex(QSP_STR(args[0])) >= 0);
            break;
        }
        case qspOpLen: {
            tos.Val.Num = qspStrLen(QSP_STR(args[0]));
            break;
        }
        case qspOpIsNum: {
            if (QSP_ISSTR(args[0].Type))
                tos.Val.Num = QSP_TOBOOL(qspIsNumber(QSP_STR(args[0])));
            else
                tos.Val.Num = QSP_TOBOOL(QSP_TRUE);
            break;
        }
        case qspOpLCase:{
            tos.Val.Str = qspGetNewText(QSP_STR(args[0]));
            tos.Val.Str->LowerString();
            break;
        }
        case qspOpUCase:{
            tos.Val.Str = qspGetNewText(QSP_STR(args[0]));
            tos.Val.Str->UpperString();
            break;
        }
        case qspOpStr:{
            tos.Val.Str = qspGetNewText(QSP_STR(args[0]));
            break;
        }
        case qspOpVal:{
            if (qspConvertVariantTo(args, QSP_TYPE_NUMBER))
                tos.Val.Num = QSP_NUM(args[0]);
            else
                tos.Val.Num = 0;
            break;
        }
        case qspOpArrSize:{
            tos.Val.Num = qspArraySize(QSP_STR(args[0]));
            break;
        }
        case qspOpTrim:{
            tos.Val.Str = qspGetNewText(qspDelSpc(QSP_STR(args[0])));
            break;
        }
        case qspOpInput:{
            tos.Val.Str = qspCallInputBox(QSP_STR(args[0]));
            break;
        }
        case qspOpRnd:{
            tos.Val.Num = qspRand() % 1000 + 1;
            break;
        }
        case qspOpCountObj:{
            tos.Val.Num = qspCurObjectsCount;
            break;
        }
        case qspOpMsecsCount:{
            tos.Val.Num = qspGetTime();
            break;
        }
        case qspOpQSPVer:{
            tos.Val.Str = (argsCount > 0 ? qspCallVersion(QSP_STR(args[0])) : qspCallVersion(qspNullString));
            break;
        }
        case qspOpUserText:{
            tos.Val.Str = (qspCurInput.Str ? qspGetNewText(qspCurInput) : qspNullString);
            break;
        }
        case qspOpCurLoc:{
            tos.Val.Str = (qspCurLoc >= 0 ? qspGetNewText(qspLocs[qspCurLoc].Name) : qspNullString);
            break;
        }
        case qspOpSelObj:{
            tos.Val.Str = (qspCurSelObject >= 0 ? qspGetNewText(qspCurObjects[qspCurSelObject].Desc) : qspNullString);
            break;
        }
        case qspOpSelAct:{
            tos.Val.Str = (qspCurSelAction >= 0 ? qspGetNewText(qspCurActions[qspCurSelAction].Desc) : qspNullString);
            break;
        }
        case qspOpMainText:{
            tos.Val.Str = (qspCurDesc.Str ? qspGetNewText(qspCurDesc) : qspNullString);
            break;
        }
        case qspOpStatText:{
            tos.Val.Str = (qspCurVars.Str ? qspGetNewText(qspCurVars) : qspNullString);
            break;
        }
        case qspOpCurActs:{
            tos.Val.Str = qspGetAllActionsAsCode();
            break;
        }
            /* External functions -------------------------------------------------------------- */
        default:{
        MathOperations[opCode]->Func(args, argsCount, &tos);
        break;
    }
    }
        //выполняем операцию, ориентируясь на её код
    if (argsCount) args.ClearVariants();
        //если аргументов было более нуля - чистим память от них
    if (qspRefreshCount != oldRefreshCount || ErrorNum) return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
        //если случились тормоза иои ошибка, выводим пустой вариант
    return tos;
        //иначе выводим результат работы функции.
}

int qsp_CompiledExpression::SkipValue(int valueIndex)
{
    if (valueIndex < 0) return -1;
    char argsCount = compArgsCounts[valueIndex];//записали в стеке счетчик аргументов.
    --valueIndex; //уменьшили индекс.
    if (argsCount)// если счетчик аргументов до уменьшения был более нуля
    {
        int i;
        for (i = 0; i < argsCount; ++i) //то циклом на количество аргументов
            valueIndex = SkipValue(valueIndex); //то делаем рекурсивный вызов на само себя.
    }
    return valueIndex;
    //суммарно, если аргументом было нечто с кучей своих аргументов, то скипнет их скопом
}

qsp_variant* qsp_CompiledExpression::ArgumentValue(int ArgIndex, char ArgType)
{
    int oldRefreshCount = qspRefreshCount; //anti-freeze?
    qsp_variant* res = Value(ArgIndex); //и тут видимо нужно чтоб "Вэлюэ" так же давал пойнтер на вариант.
    if (qspRefreshCount != oldRefreshCount || ErrorNum)
        return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
    if (QSP_ISDEF(ArgType) && !qspConvertVariantTo(&res, ArgType))//TODO: variant must have converting method
    {
        SetError(QSP_ERR_TYPEMISMATCH);
        qspFreeString(QSP_STR(res));
        return qspGetEmptyVariant(QSP_TYPE_UNDEFINED);
    }
    return res;
}
