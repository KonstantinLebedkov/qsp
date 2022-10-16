#pragma once
#include "qsp_string.h"
enum
{
    QSP_ERR_DIVBYZERO = 100,
    QSP_ERR_TYPEMISMATCH,
    QSP_ERR_STACKOVERFLOW,
    QSP_ERR_TOOMANYITEMS,
    QSP_ERR_CANTLOADFILE,
    QSP_ERR_GAMENOTLOADED,
    QSP_ERR_COLONNOTFOUND,
    QSP_ERR_CANTINCFILE,
    QSP_ERR_CANTADDACTION,
    QSP_ERR_EQNOTFOUND,
    QSP_ERR_LOCNOTFOUND,
    QSP_ERR_ENDNOTFOUND,
    QSP_ERR_LABELNOTFOUND,
    QSP_ERR_INCORRECTNAME,
    QSP_ERR_QUOTNOTFOUND,
    QSP_ERR_BRACKNOTFOUND,
    QSP_ERR_BRACKSNOTFOUND,
    QSP_ERR_SYNTAX,
    QSP_ERR_UNKNOWNACTION,
    QSP_ERR_ARGSCOUNT,
    QSP_ERR_CANTADDOBJECT,
    QSP_ERR_CANTADDMENUITEM,
    QSP_ERR_TOOMANYVARS,
    QSP_ERR_INCORRECTREGEXP,
    QSP_ERR_CODENOTFOUND,
    QSP_ERR_LOOPWHILENOTFOUND
};
class qsp_errors
{
    //TODO: move fields to private section
    //TODO: variables with curLos, line and ActIndex must be part of something other. and if they will be removed from here, init can be eliminated, and replaced with just REsetError.
public:
    int ErrorNum;
    int ErrorLoc;
    int ErrorLine;
    int ErrorActIndex;
    int RealCurLoc;
    int RealLine;
    int RealActIndex;
    static qsp_errors& ErrorsHandler();
    void Init();
    void SetError(int num);
    void ResetError();
    qsp_string qspGetErrorDesc(int errorNum);
private:
    qsp_errors() {};
    qsp_errors(const qsp_errors& root) = delete;
    qsp_errors& operator=(const qsp_errors& root) = delete;
};

