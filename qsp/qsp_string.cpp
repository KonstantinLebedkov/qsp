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
    //qsp_variants compValues; //объявляет массив вариантов, размерностью 200. нахуа?
    //std::vector<char> compOpCodes, compArgsCounts; // объявляет 2 массива чаров, размерностью 200 каждый.
    qsp_CompiledExpression CompiledExpression = this->CompileExpression();
    if (!CompiledExpression.itemsCount)
        return qspGetEmptyVariant(QSP_TYPE_UNDEFINED); //если итемс-каунт оказался нулем, тоесть фолс - инвертируем в тру и из функции выдаем эмпти-вариант, с типом "неопределен"
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
{//вот можно было б использовать find но - тут несколько символов подходящих под маску.
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
//TODO: сделать в начале копию строки и потрошить уже её. либо, в месте вызова сначала клонировать, а потом потрошить.
{
    qsp_CompiledExpression CompExpr; //объект для вывода скомпилированного кода
    CompExpr.itemsCount = 0;
    qsp_expression Expression; //стек
    //String name; //удалить здесь, потому как далее оно должно вызываться в локальных блоках и очищаться по выходу из них. между блоками не передается.
    bool waitForOperator = false; // вот она глобальная, между итерациями цикла, определяет по какой ветке пойдет цикл
    char opCode;
    int opSp = -1;//высота(заполненность) стека
    if (!Expression.PushOpCode(&opSp, qspOpStart))return CompExpr; //пушит в массивы, под номером ++opSp значения: в стек операторов - код оператора, в стек аргументов минимальное число аргументов этого оператора. в данном случае, первый вызов в этой функции - код и аргументы стартёра.
    //TODO: перевернуть в нормальное состояние - основной веткой сделать то что под ифом.
    else {
        while (1)
        {
            this->SkipSpaces();
            if (waitForOperator)
            {
                opCode = this->OperatorOpCode();//pop operator name from String and get its code : вырезали имя оператора из строки и получили его код
                if (opCode == qspOpUnknown || opCode >= qspOpFirst_Function)
                {// проверка, что полученный код оператора - приемлемый. иначе сет-эррор и брейк
                    SetError(QSP_ERR_UNKNOWNACTION);
                    break;
                }
                /* We want to separate keywords : Мы хотим чтобы слова разделялись */
                if ((opCode == qspOpAnd || opCode == qspOpOr || opCode == qspOpMod) && (this->empty() || !this->IsInClass(QSP_CHAR_SPACE | QSP_CHAR_QUOT | QSP_CHAR_EXPSTART)))
                {// проверка что после "И", "ИЛИ", "МОД" должно быть нечто отделенное пробелом либо же взятое в кавычки или скобки
                    SetError(QSP_ERR_SYNTAX);
                    break;
                }
                while (MathOperations[opCode]->Priority <= MathOperations[Expression.opStack[opSp]]->Priority && MathOperations[Expression.opStack[opSp]]->Priority != 127)
                { //так, уловил. есть код и его приоритет. пока приоритет меньше, чем у кода в стеке, и у кода в стеке он не равен 127 мы аппендим операторы из стека.
                    qsp_variant v; //TODO: должен быть или дефолтно-заполняемый вариант, или переопределенный Append, варианта не требующий.
                    if (!CompExpr.Append(Expression.opStack[opSp], Expression.argStack[opSp], v)) break;
                    // вызываем Append и если результатом она дала фолс, тоесть зиро (добавление провалилось), то инвертнув его логически, получаем тру - следовательно делаем брейк цикла парсинга. ежели дала тру - то под иф не идем и брейк не делаем. вот так следует читать сию конструкцию
                    // возвращаясь к разбору кода цикла:
                    // что мне нибуя не ясно пока - это нафига аппендить V, если оно ещё не было инициализировано.
                    if (--opSp < 0)
                    {// далее, декрементим индекс оператора в стеке (и если он вдруг оказался менее нуля, ставим ошибку и делаем брейк цикла парсинга оператора)
                        SetError(QSP_ERR_SYNTAX);
                        break;
                    }//если после декремента, дно стека не пробито - проверка приоритетов и, если надо, продолжение цикла.
                }
                if (ErrorNum) break; //если были ошибки в цикле разбора стека - то и парсинг всей строки прерываем.
                switch (opCode)
                {// если разбор стека операторов прошел без ошибок до достаточного уровня приоритета - свичуем код оператора
                    // здесь, если энд - трушный выход
                    //брекет, брекет массива, кавычка - проверка на синтаксис с обработкой
                    case qspOpEnd:{
                        if (opSp)//если opSp равен 1 для оператора энд - то флаг ошибки и брейк кейса
                        {
                            SetError(QSP_ERR_BRACKNOTFOUND);
                            break;
                        } // иначе, если opSp фолс - то энд пройден и надо возвратить скомпилированное объявление
                        return CompExpr; //вот тут трушный выход из цикла парсинга - найден энд, и стек по приоритету слит в компайлед
                    }
                    case qspOpCloseBracket: {
                        if (Expression.opStack[opSp] != qspOpOpenBracket)
                        {//если оператор - закрывающая скобка, а в стеке последним оператором НЕ парная ей открывающая = ошибка. флаг и брейк кейса
                            SetError(QSP_ERR_BRACKNOTFOUND);
                            break;
                        }
                        //иначе, скобка успешно парная.
                        opCode = Expression.opStack[--opSp]; //шаг к предыдущему коду в стеке и работа с ним
                        if (opCode >= qspOpFirst_Function && (++Expression.argStack[opSp] < MinArgsCount(opCode) || Expression.argStack[opSp] > MaxArgsCount(opCode))) SetError(QSP_ERR_ARGSCOUNT);//если фнкция по коду - стартёр, то её аргументов счетчик плюсуем, и проверяем что за пределы не вышел. если вышел, ставим ошибку.
                        break;//а если не вышел - брейкаем цикл
                    }
                    case qspOpCloseArrBracket: {
                        if (Expression.opStack[opSp] != qspOpOpenArrBracket)
                        {//встретив закрывающую массив скобу, проверили что предыдущей командой была открывающая. нет? - ошибка, и брейк(кейса)
                            SetError(QSP_ERR_BRACKNOTFOUND);
                            break;
                        }//иначе, ок, скобка была.
                        opCode = Expression.opStack[--opSp];//работаем по предыдущему коду в стеке
                        if (opCode != qspOpArrItem) 
                        {// ну и проверили, если там не элемент массива - флаг ошибки и брейк свича
                            SetError(QSP_ERR_SYNTAX);
                            break;
                        }//иначе - да, был элемент массива - всё ок.
                        Expression.argStack[opSp]++;//и если всё ок - инкрементим счетчик аргументов.
                        break;
                    }
                    case qspOpComma:{
                        if (Expression.opStack[opSp] == qspOpOpenBracket && Expression.opStack[opSp - 1] >= qspOpFirst_Function)
                        {//встретив комму, смотрим - была открывающая скобка, и перед ней стартёр
                            if (++(Expression.argStack[opSp - 1]) > MaxArgsCount(Expression.opStack[opSp - 1]))
                            {//повышаем счетчик аргументов оператора (первой функции), и ежели он превысит дозволенное количество - флаг ошибки и брейк кейса
                                SetError(QSP_ERR_ARGSCOUNT);
                                break;
                            }
                        }
                        else
                        {//а если скобки или стартёра (или того и другого) не было - пушим комму
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
                if (ErrorNum) break; //если случилась ошибка в свиче - брейкаем вайл
            }
            else
            {// если оператора не ждём - то видимо, мы его парсим.
            //и глобально, тут есть такие варианты:
            //пустой - ошибка. если строка пуста, то должен был случиться выход из функции.
            //цифирь - аппендим в комапайлед с параметрами: 3(тоесть значение), 0(тоесть аргументов ноль), соб-но цифирь
                //при том ставим флаг "ждем оператора"
            //кавычка - аппендим: 4(форматированое значение = строка), 0, собственно строка.
                //тако же ставим флаг "ждем оператора"
            //фигурная скобка "{" - аппендим: 3, 0, строка
                //и да, опять ставим флаг
            //минус - НЕ аппендим, но пушим в стек о минусе, и режем его из строки. Заметьте - НЕ ждем оператора.
            //скобка "(" - НЕ аппендим, пушим в стек. И НЕ ждем оператора
            //скобка ")" - не аппендим, не пушим, наоборот - делаем проверки и поднимаем из стека открывающую скобу. остается стартёр+ перед скобками.
            //делим(пробел, точка, запятая - типа того) - вот тут тоже интересно!
                //во-первых, тут есть упоминание неких юзер-функ, начинающихся собакой.
                    //в этом случае, аппендим строку именующую функцию, притом без собаки, и пушим код ОпФунк
                    //тако же, в стеке помечаем что аргументов там +1, и смотрим наличие открывающей скобки сразу. если таковая есть - пушим про нее, в целом, действуем как будто к ней(скобке) пришли по этой ветке
                    //нет скобки - ждем оператора
                //во-вторых, НЕ юзер-функ, тоесть не с собаки начинается.
                    // тогда код определяем по имени функции,
                        // если код = стартёр+ то
                            //смотрим скобку. есть скобка - пушим код и скобку.
                            //нет скобки - смотрим минимум аргументов
                            // больше двуъ = ошибка
                            //меньше двух - пушим код
                                // один - плюсуем аргументов (заметьте - самого аргумента ещё не видели!)
                                // ноль - ждем оператор.
                        // код меньше стартёра - аппендим: 3 (вэлюэ), 0, нейм (причем тип варианта = вар-реф)
                            //если следом скобка - режем её, и на всяк случай спейсы (могут оказаться есть)
                                //если скобка закрылась - режем её, пушим ЛастАррИтем, плюсуем каунтер аргументов и ждем оператора
                                //если не закрылась - то там элемент массива (даже если массив из одного элемента. потому - пушим АррИтем, плюсуем ему аргументов и пушим что открыта скобка массива.
                            //если не открывается скобка - то пушим АррИтем и плюсуем ему аргумент
            //иные варианты - ошибка синтаксиса.
                if (this->empty())
                {// и если при этом строка пуста
                    if (Expression.opStack[opSp] >= qspOpFirst_Function)
                        SetError(QSP_ERR_ARGSCOUNT); //а парсимый оператор типа "первой функции" - то ошибка в количестве аргументов
                    else
                        SetError(QSP_ERR_SYNTAX); //либо ошибка синтаксиса
                    break;
                }
                else if (this->IsDigit())// а если строка НЕ пуста, и при том там цыфирь
                {
                    qsp_variant v;
                    v.Type = QSP_TYPE_NUMBER;
                    *(v.Val.Num) = this->GetNumber();
                    if (Expression.opStack[opSp] == qspOpNegation)
                    {
                        *(v.Val.Num) = -*(v.Val.Num);
                        --opSp;
                    }
                    // TODO: выше, и вытаскивание, и назначение минуса убрать под один тернарник.
                    if (!CompExpr.Append(qspOpValue, 0, v))break;
                    waitForOperator = true;
                }
                else if (this->IsInClass( QSP_CHAR_QUOT))
                {//строка не пуста, и там кавычка, тоесть начало чего-то типа строковой константы
                    qsp_variant v;
                    v.Type = QSP_TYPE_STRING;
                    v.Val.Str->assign(GetString());
                    if (ErrorNum) break;
                    if (!CompExpr.Append(qspOpValueToFormat,0,v)) break;
                    waitForOperator = true;
                }
                else if (*this->begin() == QSP_LQUOT[0]) // "{"
                {// строка не пуста, и там открывающая фигурная скобка
                    qsp_variant v;
                    v.Type = QSP_TYPE_CODE;
                    v.Val.Str->assign(this->GetQString());
                    if (ErrorNum) break;
                    //TODO: по хорошему, тут и вариант надо заявлять/заполнять стразу внутри вызова оного Append. погуглить как это делается
                    if (!CompExpr.Append(qspOpValue,0,v))
                    {
                        //qspFreeString(QSP_STR(v)); //см выше, вариант объявлен локально в зоне видимости блока и потому не требует спец-чистки. его деструктор зовется автоматом, и в деструкторе удаление троки записано раз уж он строковый. upd! проверил и добавил, что убивать строку на которую идет ссылка надо и в случае если типом варианта является код.
                        break;
                    }
                    waitForOperator = true;
                }
                else if (*this->begin() == QSP_NEGATION[0]) // "-"
                {
                    if (!Expression.PushOpCode(&opSp, qspOpNegation)) break;
                    s.Str += QSP_STATIC_LEN(QSP_NEGATION);
                    //TODO: здесь просто дропается первый символ. добавить строке метод "дропнуть первых Н штук" и юзать его здесь.
                }
                else if (*this->begin() == QSP_LRBRACK[0])// "("
                {
                    if (!Expression.PushOpCode(&opSp, qspOpOpenBracket)) break;
                    s.Str += QSP_STATIC_LEN(QSP_LRBRACK); //см выше - дропнуть первых
                }
                else if (*this->begin() == QSP_RRBRACK[0]) // ")"
                {// и вот тут интересно. во первых, в код поднимаем значение кода из стека
                    opCode = Expression.opStack[opSp];
                    if (opCode != qspOpOpenBracket)
                    {// и если оказалось, что там НЕ открывающая скобка - ставим ошибку. если там стартёр+ то ошибка счётчика аргументов, а иначе - ошибка синтаксиса
                        if (opCode >= qspOpFirst_Function)
                            SetError(QSP_ERR_ARGSCOUNT);
                        else
                            SetError(QSP_ERR_SYNTAX);
                        break;
                    }
                    opCode = Expression.opStack[--opSp];// окей, открывающая скобка была. тогда поднимаем код перед ней.
                    if (opCode < qspOpFirst_Function)
                    {// и если там было что-то меньше стартёра - ошибка синтаксиса
                        SetError(QSP_ERR_SYNTAX);
                        break;
                    }
                    // окей, перед открывающей скобой был стартёр+
                    if (Expression.argStack[opSp] < MinArgsCount(opCode))
                    {//смотрим счетчик аргументов этого стартёра+, и если число аргументов не дотянуло до минимума - брейк с ошибкой.
                        SetError(QSP_ERR_ARGSCOUNT);
                        break;
                    }
                    s.Str += QSP_STATIC_LEN(QSP_RRBRACK);
                    waitForOperator = true;
                    //ну и наконец, все проверки выполнены, указатель стека всё так же показывает на стартёр+ перед скобой, с неким количеством аргументов. режем из строки закрывающую скобу и ждем следующего оператора.
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
        //а сей вайл есть очистка памяти от набитых вариантов. и должна она быть реализована как метод.
        //TODO: вынести его куда следует методом.
        while (--(CompExpr.itemsCount) >= 0)
        {//в сучности, что происходит в цикле: по массиву, ориентируясь на счетчик и пока он более нуля, для каждого компайлед оперейшен кодес, свичуем: если код оказался вэлюэ, вили валюэ-ту-формат, то для этого итема, его вэоюе проверяем - строка ли оно. и ежели строка, то строку опустошаем.
        //апдейд: на итемс-каунт тут влияет только (тадамс!) апенд, который есть метод компайлед экспрессьона. который значение в сучности только инкрементит. ну и да, в массив вариантов он значение кладет только когда код - либо ОП-вэлюэ, либо Оп-Вэлюэ-Ту-Формат.
        //тоесть, это в сучности, очистка памяти. и тады цикл завершается выдав минус адын. причем это минус адын в нативе было результатом. видимо, означающим фейл. тоесть саксесс - НЕотрицательное значение сего каунтера.
        // а неотрицательным оно может быть только если этот цикл мы вообще вот не посещаем, и очистка набитых вариантов делается где-то ищщо. а выход из функции происходит не в конце тела функции ретюрном, а подобным ретюрном вызванным где-то раньше, в теле. в первом цикле, видимо.
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
    if (this->empty()) return qspOpEnd;// возвращает код оператора. имеет смысл заявить их(коды) в отдельном хедере?
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