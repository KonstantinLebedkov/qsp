#pragma once
#include <string>
#include "qsp_expression.h"


#define QSP_OPSLEVELS 2

#define String qsp_string
#define SIterator qsp_string::iterator

enum
{
    QSP_CHAR_SPACE = 1 << 0, /* QSP_SPACES */
    QSP_CHAR_QUOT = 1 << 1, /* QSP_QUOTS */
    QSP_CHAR_DELIM = 1 << 2, /* QSP_DELIMS */
    QSP_CHAR_SIMPLEOP = 1 << 3, /* QSP_ADD QSP_SUB QSP_DIV QSP_MUL */
    QSP_CHAR_EXPSTART = 1 << 4 /* QSP_LQUOT QSP_LRBRACK QSP_LSBRACK */
};

static const unsigned char qspAsciiClasses[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x04, 0x06, 0x00, 0x00, 0x00, 0x04, 0x06,
    0x14, 0x04, 0x0c, 0x0c, 0x04, 0x0c, 0x00, 0x0c,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x00, 0x04, 0x04, 0x04, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x14, 0x00, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x14, 0x00, 0x04, 0x00, 0x00,
};

class qsp_string : public std::wstring
{
private:
    bool replaceEntryText(const String from, const String to);
    bool replaceEntryText(const std::wstring from, const std::wstring to);
    void SkipSpaces();
public:
    //TODO: constructors required - default and assigning
    String() {};
    String(std::wstring str) { this->assign(str); };
    ~String(){};
    String replaceText(const String from, const String to); //gives string wiht all entries replaced by example
    String replaceText(const std::wstring from, const std::wstring to);//gives string wiht all entries replaced by example
    void replaceTextInSelf(const String from, const String to);//replace inself all entries by example
    void replaceTextInSelf(const std::wstring from, const std::wstring to);//replace inself all entries by example
    bool IsAnyString();//Skip first spaces if existing and check for existing something else. It doesnt change source string
    int ToInteger();//convert to integer, to 0 if convertation impossible. It doesnt change source string.
    int GetNumber();
    void UpperString();//convert UpCase
    void LowerString();//convert LowCase
    void DeleteSpaces(); //remove starting and ending spaces
    void ExprValue(); //пока войд, а вообще должен возвращать кусп-вариант.
    bool IsDigit(); //проверяет первый символ. истина, если таковой цифра от 0 до 9
    bool IsInClass(char charClass); //check beginning symbol of the string
    String GetString(); //gets string rounded by quotes, and cut it out with quotes
    String GetName();
    //TODO: String GetQString();
private:
    qsp_CompiledExpression CompileExpression();
    char OperatorOpCode(); //give code of operator and cut it out from the string.
public:
    String& operator=(const String& source);
    SIterator DelimPos(wchar_t ch)
};
