#include "qsp_errors.h"

qsp_errors& qsp_errors::ErrorsHandler()
{
    static qsp_errors theInstance;
    return theInstance;
}

void qsp_errors::Init()
{
    int ErrorNum = 0;
    int ErrorLoc = -1;
    int ErrorLine = 0;
    int ErrorActIndex = -1;
    int RealCurLoc = -1;
    int RealLine = 0;
    int RealActIndex = -1;
}

void qsp_errors::setError(int num)
{
    if (!ErrorNum)
    {
        ErrorNum = num;
        ErrorLoc = RealCurLoc;
        ErrorActIndex = RealActIndex;
        ErrorLine = RealLine;
    }
}

void qsp_errors::ResetError()
{
    ErrorNum = 0;
    ErrorLoc = -1;
    ErrorActIndex = -1;
    ErrorLine = 0;
}

qsp_string qsp_errors::qspGetErrorDesc(int errorNum)
{
    qsp_string str;
    switch (errorNum)
    {
    case QSP_ERR_DIVBYZERO: str = L"Division by zero!"; break;
    case QSP_ERR_TYPEMISMATCH: str = L"Type mismatch!"; break;
    case QSP_ERR_STACKOVERFLOW: str = L"Stack overflow!"; break;
    case QSP_ERR_TOOMANYITEMS: str = L"Too many items in expression!"; break;
    case QSP_ERR_CANTLOADFILE: str = L"Can't load file!"; break;
    case QSP_ERR_GAMENOTLOADED: str = L"Game not loaded!"; break;
    case QSP_ERR_COLONNOTFOUND: str = L"Sign [:] not found!"; break;
    case QSP_ERR_CANTINCFILE: str = L"Can't add file!"; break;
    case QSP_ERR_CANTADDACTION: str = L"Can't add action!"; break;
    case QSP_ERR_EQNOTFOUND: str = L"Sign [=] not found!"; break;
    case QSP_ERR_LOCNOTFOUND: str = L"Location not found!"; break;
    case QSP_ERR_ENDNOTFOUND: str = L"[end] not found!"; break;
    case QSP_ERR_LABELNOTFOUND: str = L"Label not found!"; break;
    case QSP_ERR_INCORRECTNAME: str = L"Incorrect variable name!"; break;
    case QSP_ERR_QUOTNOTFOUND: str = L"Quote not found!"; break;
    case QSP_ERR_BRACKNOTFOUND: str = L"Bracket not found!"; break;
    case QSP_ERR_BRACKSNOTFOUND: str = L"Brackets not found!"; break;
    case QSP_ERR_SYNTAX: str = L"Syntax error!"; break;
    case QSP_ERR_UNKNOWNACTION: str = L"Unknown action!"; break;
    case QSP_ERR_ARGSCOUNT: str = L"Incorrect arguments' count!"; break;
    case QSP_ERR_CANTADDOBJECT: str = L"Can't add object!"; break;
    case QSP_ERR_CANTADDMENUITEM: str = L"Can't add menu item!"; break;
    case QSP_ERR_TOOMANYVARS: str = L"Too many variables!"; break;
    case QSP_ERR_INCORRECTREGEXP: str = L"Regular expression error!"; break;
    case QSP_ERR_CODENOTFOUND: str = L"Code not found!"; break;
    case QSP_ERR_LOOPWHILENOTFOUND: str = L"Loop condition not found!"; break;
    default: str = L"Unknown error!"; break;
    }
    return str;
}


