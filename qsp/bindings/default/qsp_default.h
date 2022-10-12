/* Copyright (C) 2001-2020 Valeriy Argunov (byte AT qsp DOT org) */
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/
#pragma once
    static int qspEndiannessTestValue = 1;

    #ifdef _UNICODE
        typedef wchar_t QSP_CHAR;
        #define QSP_FMT2(x) L##x
        #define QSP_FMT(x) QSP_FMT2(x)

        #define QSP_ONIG_ENC ((*(char *)&(qspEndiannessTestValue) == 1) ? \
                    (sizeof(QSP_CHAR) == 2 ? ONIG_ENCODING_UTF16_LE : ONIG_ENCODING_UTF32_LE) : \
                    (sizeof(QSP_CHAR) == 2 ? ONIG_ENCODING_UTF16_BE : ONIG_ENCODING_UTF32_BE))
        #define QSP_FROM_OS_CHAR(a) qspReverseConvertUC(a, qspCP1251ToUnicodeTable)
        #define QSP_TO_OS_CHAR(a) qspDirectConvertUC(a, qspCP1251ToUnicodeTable)
        #define QSP_CHRLWR qspToWLower
        #define QSP_CHRUPR qspToWUpper
        #define QSP_WCTOB
        #define QSP_BTOWC
    #else
        typedef char QSP_CHAR;
        #define QSP_FMT(x) x

        #if defined(WIN32)
            #define QSP_ONIG_ENC ONIG_ENCODING_CP1251
            #define QSP_FROM_OS_CHAR
            #define QSP_TO_OS_CHAR
            #define QSP_CHRLWR(a) qspCP1251ToLowerTable[(unsigned char)(a)]
            #define QSP_CHRUPR(a) qspCP1251ToUpperTable[(unsigned char)(a)]
            #define QSP_WCTOB(a) qspReverseConvertUC(a, qspCP1251ToUnicodeTable)
            #define QSP_BTOWC(a) qspDirectConvertUC(a, qspCP1251ToUnicodeTable)
        #else
            #define QSP_ONIG_ENC ONIG_ENCODING_KOI8_R
            #define QSP_FROM_OS_CHAR(a) qspReverseConvertSB(a, qspCP1251ToKOI8RTable)
            #define QSP_TO_OS_CHAR(a) qspDirectConvertSB(a, qspCP1251ToKOI8RTable)
            #define QSP_CHRLWR(a) qspKOI8RToLowerTable[(unsigned char)(a)]
            #define QSP_CHRUPR(a) qspKOI8RToUpperTable[(unsigned char)(a)]
            #define QSP_WCTOB(a) qspReverseConvertUC(a, qspKOI8RToUnicodeTable)
            #define QSP_BTOWC(a) qspDirectConvertUC(a, qspKOI8RToUnicodeTable)
        #endif
    #endif

    #define QSP_FIXBYTESORDER(a) ((*(char *)&(qspEndiannessTestValue) == 1) ? \
                                 (a) : \
                                 ((unsigned short)(((a) << 8) | ((a) >> 8))))
    #if defined(_MSC_VER)
        #define QSP_TIME _time64
    #else
        #define QSP_TIME time
    #endif

    #ifdef __cplusplus
        typedef int (*QSP_CALLBACK)(...);
    #else
        typedef int (*QSP_CALLBACK)();
    #endif

    #include "../qsp.h"

    #ifdef __cplusplus
        extern "C" {
    #endif

    extern void QSPInit();
    extern void QSPDeInit();
    extern void QSPSetCallBack(int type, QSP_CALLBACK func);
    extern void QSPEnableDebugMode(QSP_BOOL isDebug);
    extern void QSPGetCurStateData(QSPString *loc, int *actIndex, int *line);
    extern QSPString QSPGetVersion();
    extern QSPString QSPGetCompiledDateTime();
    extern int QSPGetFullRefreshCount();
    /* Main desc */
    extern QSPString QSPGetMainDesc();
    extern QSP_BOOL QSPIsMainDescChanged();
    /* Vars desc */
    extern QSPString QSPGetVarsDesc();
    extern QSP_BOOL QSPIsVarsDescChanged();
    /* Input string */
    extern void QSPSetInputStrText(QSPString str);
    /* Actions */
    extern int QSPGetActions(QSPListItem *items, int itemsBufSize);
    extern QSP_BOOL QSPSetSelActionIndex(int ind, QSP_BOOL isRefresh);
    extern int QSPGetSelActionIndex();
    extern QSP_BOOL QSPIsActionsChanged();
    extern QSP_BOOL QSPExecuteSelActionCode(QSP_BOOL isRefresh);
    /* Objects */
    extern int QSPGetObjects(QSPListItem *items, int itemsBufSize);
    extern QSP_BOOL QSPSetSelObjectIndex(int ind, QSP_BOOL isRefresh);
    extern int QSPGetSelObjectIndex();
    extern QSP_BOOL QSPIsObjectsChanged();
    /* Windows */
    extern void QSPShowWindow(int type, QSP_BOOL isShow);
    /* Code execution */
    extern QSP_BOOL QSPExecString(QSPString str, QSP_BOOL isRefresh);
    extern QSP_BOOL QSPExecCounter(QSP_BOOL isRefresh);
    extern QSP_BOOL QSPExecUserInput(QSP_BOOL isRefresh);
    extern QSP_BOOL QSPExecLocationCode(QSPString name, QSP_BOOL isRefresh);
    /* Errors */
    extern void QSPGetLastErrorData(int *errorNum, QSPString *errorLoc, int *errorActIndex, int *errorLine);
    extern QSPString QSPGetErrorDesc(int errorNum);
    /* Game */
    extern QSP_BOOL QSPLoadGameWorldFromData(const void *data, int dataSize, QSP_BOOL isNewGame);
    extern QSP_BOOL QSPSaveGameAsData(void *buf, int *bufSize, QSP_BOOL isRefresh);
    extern QSP_BOOL QSPOpenSavedGameFromData(const void *data, int dataSize, QSP_BOOL isRefresh);

    extern QSP_BOOL QSPRestartGame(QSP_BOOL isRefresh);
    /* Variables */
    extern QSP_BOOL QSPGetVarValuesCount(QSPString name, int *count);
    extern QSP_BOOL QSPGetVarValues(QSPString name, int ind, int *numVal, QSPString *strVal);
    extern int QSPGetMaxVarsCount();
    extern QSP_BOOL QSPGetVarNameByIndex(int ind, QSPString *name);

    #ifdef __cplusplus
    }
    #endif