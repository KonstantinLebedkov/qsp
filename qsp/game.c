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

#include "game.h"
#include "actions.h"
#include "callbacks.h"
#include "codetools.h"
#include "coding.h"
#include "common.h"
#include "errors.h"
#include "locations.h"
#include "objects.h"
#include "playlist.h"
#include "statements.h"
#include "text.h"
#include "time.h"
#include "variables.h"

int qspQstCRC = 0;

QSPString qspCurIncFiles[QSP_MAXINCFILES];
int qspCurIncFilesCount = 0;
int qspCurIncLocsCount = 0;

int qspCRCTable[256] =
{
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

INLINE int qspCRC(void *, int);
INLINE void qspIncludeFile(QSPString s);
INLINE void qspOpenIncludes();
INLINE QSP_BOOL qspCheckGame(QSPString *strs, int count);
INLINE QSP_BOOL qspSkipLines(int *index, int totalLinesCount, int linesToSkip);
INLINE QSP_BOOL qspGetIntValueAndSkipLine(int *value, int *index, QSPString *strs, int totalLinesCount);
INLINE QSP_BOOL qspCheckGameStatus(QSPString *strs, int strsCount);

INLINE int qspCRC(void *data, int len)
{
    unsigned char *ptr;
    int crc = 0;
    ptr = (unsigned char *)data;
    while (len--)
        crc = (qspCRCTable[(crc & 0xFF) ^ *ptr++] ^ crc >> 8) ^ 0xD202EF8D;
    return crc;
}

void qspClearIncludes(QSP_BOOL isFirst)
{
    int i, count;
    if (!isFirst)
    {
        for (i = 0; i < qspCurIncFilesCount; ++i)
            qspFreeString(qspCurIncFiles[i]);
        if (qspCurIncLocsCount)
        {
            count = qspLocsCount - qspCurIncLocsCount;
            qspCreateWorld(count, count);
            qspPrepareLocs();
        }
    }
    qspCurIncFilesCount = 0;
    qspCurIncLocsCount = 0;
}

INLINE void qspIncludeFile(QSPString s)
{
    int i, oldRefreshCount;
    if (!qspIsAnyString(s)) return;
    if (qspCurIncFilesCount == QSP_MAXINCFILES)
    {
        qspSetError(QSP_ERR_CANTINCFILE);
        return;
    }
    for (i = 0; i < qspCurIncFilesCount; ++i)
    {
        if (!qspStrsComp(qspCurIncFiles[i], s))
            return;
    }
    oldRefreshCount = qspRefreshCount;
    qspCallOpenGame(s, QSP_FALSE);
    if (qspRefreshCount != oldRefreshCount || qspErrorNum) return;
    qspCurIncFiles[qspCurIncFilesCount++] = qspGetNewText(s);
}

INLINE void qspOpenIncludes()
{
    int i, oldRefreshCount = qspRefreshCount;
    for (i = 0; i < qspCurIncFilesCount; ++i)
    {
        qspCallOpenGame(qspCurIncFiles[i], QSP_FALSE);
        if (qspRefreshCount != oldRefreshCount || qspErrorNum) return;
    }
}

void qspNewGame(QSP_BOOL isReset)
{
    if (!qspLocsCount)
    {
        qspSetError(QSP_ERR_GAMENOTLOADED);
        return;
    }
    qspCurLoc = 0;
    if (isReset)
    {
        qspSetSeed((unsigned int)QSP_TIME(0));
        qspTimerInterval = QSP_DEFTIMERINTERVAL;
        qspCurIsShowObjs = qspCurIsShowActs = qspCurIsShowVars = qspCurIsShowInput = QSP_TRUE;
        qspMemClear(QSP_FALSE);
        qspResetTime(0);
        qspCallShowWindow(QSP_WIN_ACTS, QSP_TRUE);
        qspCallShowWindow(QSP_WIN_OBJS, QSP_TRUE);
        qspCallShowWindow(QSP_WIN_VARS, QSP_TRUE);
        qspCallShowWindow(QSP_WIN_INPUT, QSP_TRUE);
        qspCallSetInputStrText(qspNullString);
        qspCallShowPicture(qspNullString);
        qspCallCloseFile(qspNullString);
        qspCallSetTimer(QSP_DEFTIMERINTERVAL);
    }
    qspRefreshCurLoc(QSP_TRUE, 0, 0);
}

INLINE QSP_BOOL qspCheckGame(QSPString *strs, int count)
{
    int i, ind, locsCount, actsCount;
    QSP_BOOL isOldFormat = qspStrsComp(strs[0], QSP_STATIC_STR(QSP_GAMEID)) != 0;
    ind = (isOldFormat ? 30 : 4);
    if (ind >= count) return QSP_FALSE;
    locsCount = (isOldFormat ? qspStrToNum(strs[0], 0) : qspDeCodeGetIntVal(strs[3]));
    if (locsCount <= 0) return QSP_FALSE;
    for (i = 0; i < locsCount; ++i)
    {
        if ((ind += 3) >= count) return QSP_FALSE;
        if (isOldFormat)
            actsCount = 20;
        else
        {
            if (ind >= count) return QSP_FALSE;
            actsCount = qspDeCodeGetIntVal(strs[ind++]);
            if (actsCount < 0 || actsCount > QSP_MAXACTIONS) return QSP_FALSE;
        }
        if ((ind += (actsCount * (isOldFormat ? 2 : 3))) >= count) return QSP_FALSE;
    }
    return QSP_TRUE;
}

QSP_BOOL qspOpenGame(void *data, int dataSize, QSP_BOOL isNewGame)
{
    QSP_BOOL isOldFormat, isAddLoc, isUCS;
    int i, j, ind, crc, count, locsCount, actsCount, start, end;
    QSPString buf, gameString, *strs;
    if (isNewGame) crc = qspCRC(data, dataSize);
    isUCS = (dataSize >= 2 && *((char *)data + 1) == 0);
    gameString = qspStringFromFileData(data, dataSize, isUCS);
    count = qspSplitStr(gameString, QSP_STATIC_STR(QSP_STRSDELIM), &strs);
    qspFreeString(gameString);
    if (!qspCheckGame(strs, count))
    {
        qspSetError(QSP_ERR_CANTLOADFILE);
        qspFreeStrs(strs, count);
        return QSP_FALSE;
    }
    isOldFormat = qspStrsComp(strs[0], QSP_STATIC_STR(QSP_GAMEID)) != 0;
    locsCount = (isOldFormat ? qspStrToNum(strs[0], 0) : qspDeCodeGetIntVal(strs[3]));
    if (isNewGame)
    {
        qspClearIncludes(QSP_FALSE);
        start = 0;
        end = locsCount;
    }
    else
    {
        start = qspLocsCount;
        end = start + locsCount;
    }
    locsCount = qspLocsCount;
    qspCreateWorld(start, end);
    qspLocsCount = locsCount;
    locsCount = start;
    ind = (isOldFormat ? 30 : 4);
    for (i = start; i < end; ++i)
    {
        buf = qspCodeDeCode(strs[ind++], QSP_FALSE);
        if (isAddLoc = (isNewGame || qspLocIndex(buf) < 0))
            qspLocs[locsCount].Name = buf;
        else
            qspFreeString(buf);
        if (isAddLoc)
        {
            qspLocs[locsCount].Desc = qspCodeDeCode(strs[ind++], QSP_FALSE);
            buf = qspCodeDeCode(strs[ind++], QSP_FALSE);
            qspLocs[locsCount].OnVisitLinesCount = qspPreprocessData(buf, &qspLocs[locsCount].OnVisitLines);
            qspFreeString(buf);
        }
        else
            ind += 2;
        actsCount = (isOldFormat ? 20 : qspDeCodeGetIntVal(strs[ind++]));
        if (isAddLoc)
        {
            for (j = 0; j < actsCount; ++j)
            {
                qspLocs[locsCount].Actions[j].Image = (isOldFormat ? qspNullString : qspCodeDeCode(strs[ind++], QSP_FALSE));
                qspLocs[locsCount].Actions[j].Desc = qspCodeDeCode(strs[ind++], QSP_FALSE);
                buf = qspCodeDeCode(strs[ind++], QSP_FALSE);
                qspLocs[locsCount].Actions[j].OnPressLinesCount = qspPreprocessData(buf, &qspLocs[locsCount].Actions[j].OnPressLines);
                qspFreeString(buf);
            }
            ++locsCount;
        }
        else
            ind += actsCount * (isOldFormat ? 2 : 3);
    }
    qspFreeStrs(strs, count);
    qspLocsCount = end;
    qspCreateWorld(end, locsCount);
    count = locsCount - start;
    if (count) qspPrepareLocs();
    if (isNewGame)
    {
        qspQstCRC = crc;
        qspCurLoc = qspRealCurLoc = -1;
    }
    else
        qspCurIncLocsCount += count;
    return QSP_TRUE;
}

QSP_BOOL qspSaveGameStatus(void *buf, int *bufSize)
{
    void *gameData;
    QSPString bufString, locName;
    QSPVar *savedVars;
    int i, j, dataSize, varsCount, oldRefreshCount = qspRefreshCount;
    qspExecLocByVarNameWithArgs(QSP_STATIC_STR(QSP_FMT("ONGSAVE")), 0, 0);
    if (qspRefreshCount != oldRefreshCount || qspErrorNum)
    {
        *bufSize = 0;
        return QSP_FALSE;
    }
    varsCount = qspSaveLocalVarsAndRestoreGlobals(&savedVars);
    if (qspErrorNum)
    {
        *bufSize = 0;
        return QSP_FALSE;
    }
    bufString = qspNullString;
    qspRefreshPlayList();
    locName = (qspCurLoc >= 0 ? qspLocs[qspCurLoc].Name : qspNullString);
    qspCodeWriteVal(&bufString, QSP_STATIC_STR(QSP_SAVEDGAMEID), QSP_FALSE);
    qspCodeWriteVal(&bufString, QSP_STATIC_STR(QSP_VER), QSP_FALSE);
    qspCodeWriteIntVal(&bufString, qspQstCRC, QSP_TRUE);
    qspCodeWriteIntVal(&bufString, qspGetTime(), QSP_TRUE);
    qspCodeWriteIntVal(&bufString, qspCurSelAction, QSP_TRUE);
    qspCodeWriteIntVal(&bufString, qspCurSelObject, QSP_TRUE);
    qspCodeWriteVal(&bufString, qspViewPath, QSP_TRUE);
    qspCodeWriteVal(&bufString, qspCurInput, QSP_TRUE);
    qspCodeWriteVal(&bufString, qspCurDesc, QSP_TRUE);
    qspCodeWriteVal(&bufString, qspCurVars, QSP_TRUE);
    qspCodeWriteVal(&bufString, locName, QSP_TRUE);
    qspCodeWriteIntVal(&bufString, (int)qspCurIsShowActs, QSP_TRUE);
    qspCodeWriteIntVal(&bufString, (int)qspCurIsShowObjs, QSP_TRUE);
    qspCodeWriteIntVal(&bufString, (int)qspCurIsShowVars, QSP_TRUE);
    qspCodeWriteIntVal(&bufString, (int)qspCurIsShowInput, QSP_TRUE);
    qspCodeWriteIntVal(&bufString, qspTimerInterval, QSP_TRUE);
    qspCodeWriteIntVal(&bufString, qspPLFilesCount, QSP_TRUE);
    for (i = 0; i < qspPLFilesCount; ++i)
        qspCodeWriteVal(&bufString, qspPLFiles[i], QSP_TRUE);
    qspCodeWriteIntVal(&bufString, qspCurIncFilesCount, QSP_TRUE);
    for (i = 0; i < qspCurIncFilesCount; ++i)
        qspCodeWriteVal(&bufString, qspCurIncFiles[i], QSP_TRUE);
    qspCodeWriteIntVal(&bufString, qspCurActionsCount, QSP_TRUE);
    for (i = 0; i < qspCurActionsCount; ++i)
    {
        qspCodeWriteVal(&bufString, qspCurActions[i].Image, QSP_TRUE);
        qspCodeWriteVal(&bufString, qspCurActions[i].Desc, QSP_TRUE);
        qspCodeWriteIntVal(&bufString, qspCurActions[i].OnPressLinesCount, QSP_TRUE);
        for (j = 0; j < qspCurActions[i].OnPressLinesCount; ++j)
        {
            qspCodeWriteVal(&bufString, qspCurActions[i].OnPressLines[j].Str, QSP_TRUE);
            qspCodeWriteIntVal(&bufString, qspCurActions[i].OnPressLines[j].LineNum, QSP_TRUE);
        }
        qspCodeWriteIntVal(&bufString, qspCurActions[i].Location, QSP_TRUE);
        qspCodeWriteIntVal(&bufString, qspCurActions[i].ActIndex, QSP_TRUE);
    }
    qspCodeWriteIntVal(&bufString, qspCurObjectsCount, QSP_TRUE);
    for (i = 0; i < qspCurObjectsCount; ++i)
    {
        qspCodeWriteVal(&bufString, qspCurObjects[i].Image, QSP_TRUE);
        qspCodeWriteVal(&bufString, qspCurObjects[i].Desc, QSP_TRUE);
    }
    qspCodeWriteIntVal(&bufString, qspGetVarsCount(), QSP_TRUE);
    for (i = 0; i < QSP_VARSCOUNT; ++i)
    {
        if (qspVars[i].Name.Str)
        {
            qspCodeWriteIntVal(&bufString, i, QSP_TRUE);
            qspCodeWriteVal(&bufString, qspVars[i].Name, QSP_TRUE);
            qspCodeWriteIntVal(&bufString, qspVars[i].ValsCount, QSP_TRUE);
            for (j = 0; j < qspVars[i].ValsCount; ++j)
            {
                qspCodeWriteIntVal(&bufString, qspVars[i].Values[j].Type, QSP_TRUE);
                if (QSP_ISSTR(qspVars[i].Values[j].Type))
                    qspCodeWriteVal(&bufString, QSP_STR(qspVars[i].Values[j]), QSP_TRUE);
                else
                    qspCodeWriteIntVal(&bufString, QSP_NUM(qspVars[i].Values[j]), QSP_TRUE);
            }
            qspCodeWriteIntVal(&bufString, qspVars[i].IndsCount, QSP_TRUE);
            for (j = 0; j < qspVars[i].IndsCount; ++j)
            {
                qspCodeWriteIntVal(&bufString, qspVars[i].Indices[j].Index, QSP_TRUE);
                qspCodeWriteVal(&bufString, qspVars[i].Indices[j].Str, QSP_TRUE);
            }
        }
    }
    qspRestoreLocalVars(savedVars, varsCount, qspSavedVarGroups, qspSavedVarGroupsCount);
    if (qspErrorNum)
    {
        qspFreeString(bufString);
        *bufSize = 0;
        return QSP_FALSE;
    }
    gameData = qspStringToFileData(bufString, QSP_TRUE, &dataSize);
    qspFreeString(bufString);
    if (dataSize > *bufSize)
    {
        *bufSize = dataSize;
        free(gameData);
        return QSP_FALSE;
    }
    memcpy(buf, gameData, dataSize);
    *bufSize = dataSize;
    free(gameData);
    return QSP_TRUE;
}

INLINE QSP_BOOL qspSkipLines(int *index, int totalLinesCount, int linesToSkip)
{
    if ((*index += linesToSkip) >= totalLinesCount) return QSP_FALSE;
    return QSP_TRUE;
}

INLINE QSP_BOOL qspGetIntValueAndSkipLine(int *value, int *index, QSPString *strs, int totalLinesCount)
{
    *value = qspDeCodeGetIntVal(strs[*index]);
    return qspSkipLines(index, totalLinesCount, 1);
}

INLINE QSP_BOOL qspCheckGameStatus(QSPString *strs, int strsCount)
{
    int i, j, ind, count, groupsCount, varValuesCount, lastInd, temp, selAction, selObject;
    ind = 16;
    if (ind > strsCount) return QSP_FALSE;
    if (qspStrsComp(strs[0], QSP_STATIC_STR(QSP_SAVEDGAMEID)) ||
        qspStrsComp(strs[1], QSP_STATIC_STR(QSP_GAMEMINVER)) < 0 ||
        qspStrsComp(strs[1], QSP_STATIC_STR(QSP_VER)) > 0) return QSP_FALSE;
    if (!qspGetVarNumValue(QSP_STATIC_STR(QSP_FMT("DEBUG"))) &&
        qspDeCodeGetIntVal(strs[2]) != qspQstCRC) return QSP_FALSE;
    selAction = qspDeCodeGetIntVal(strs[4]); /* qspCurSelAction */
    selObject = qspDeCodeGetIntVal(strs[5]); /* qspCurSelObject */
    if (qspDeCodeGetIntVal(strs[15]) < 0) return QSP_FALSE; /* qspTimerInterval */
    /* qspPLFilesCount */
    if (!qspGetIntValueAndSkipLine(&count, &ind, strs, strsCount)) return QSP_FALSE;
    if (count < 0 || count > QSP_MAXPLFILES) return QSP_FALSE;
    /* files */
    if (!qspSkipLines(&ind, strsCount, count)) return QSP_FALSE;
    /* qspCurIncFilesCount */
    if (!qspGetIntValueAndSkipLine(&count, &ind, strs, strsCount)) return QSP_FALSE;
    if (count < 0 || count > QSP_MAXINCFILES) return QSP_FALSE;
    /* includes */
    if (!qspSkipLines(&ind, strsCount, count)) return QSP_FALSE;
    /* qspCurActionsCount */
    if (!qspGetIntValueAndSkipLine(&count, &ind, strs, strsCount)) return QSP_FALSE;
    if (count < 0 || count > QSP_MAXACTIONS || selAction >= count) return QSP_FALSE;
    /* actions */
    for (i = 0; i < count; ++i)
    {
        /* image + description */
        if (!qspSkipLines(&ind, strsCount, 2)) return QSP_FALSE;
        /* lines of code count */
        if (!qspGetIntValueAndSkipLine(&groupsCount, &ind, strs, strsCount)) return QSP_FALSE;
        if (groupsCount < 0) return QSP_FALSE;
        /* lines */
        for (j = 0; j < groupsCount; ++j)
        {
            /* line of code */
            if (!qspSkipLines(&ind, strsCount, 1)) return QSP_FALSE;
            /* line number */
            if (!qspGetIntValueAndSkipLine(&temp, &ind, strs, strsCount)) return QSP_FALSE;
            if (temp < 0) return QSP_FALSE;
        }
        /* action's location */
        if (!qspGetIntValueAndSkipLine(&temp, &ind, strs, strsCount)) return QSP_FALSE;
        if (temp < 0) return QSP_FALSE;
        /* action's source index */
        if (!qspSkipLines(&ind, strsCount, 1)) return QSP_FALSE;
    }
    /* qspCurObjectsCount */
    if (!qspGetIntValueAndSkipLine(&count, &ind, strs, strsCount)) return QSP_FALSE;
    if (count < 0 || count > QSP_MAXOBJECTS || selObject >= count) return QSP_FALSE;
    /* objects: image + description */
    if (!qspSkipLines(&ind, strsCount, 2 * count)) return QSP_FALSE;
    /* variables count */
    if (!qspGetIntValueAndSkipLine(&count, &ind, strs, strsCount)) return QSP_FALSE;
    if (count < 0) return QSP_FALSE;
    lastInd = -1;
    /* variables */
    for (i = 0; i < count; ++i)
    {
        /* variable's index */
        if (!qspGetIntValueAndSkipLine(&temp, &ind, strs, strsCount)) return QSP_FALSE;
        if (temp <= lastInd || temp >= QSP_VARSCOUNT) return QSP_FALSE;
        lastInd = temp;
        /* variable's name */
        if (!qspSkipLines(&ind, strsCount, 1)) return QSP_FALSE;
        /* values count */
        if (!qspGetIntValueAndSkipLine(&varValuesCount, &ind, strs, strsCount)) return QSP_FALSE;
        if (varValuesCount < 0) return QSP_FALSE;
        /* values */
        for (j = 0; j < varValuesCount; ++j)
        {
            /* var type */
            if (!qspGetIntValueAndSkipLine(&temp, &ind, strs, strsCount)) return QSP_FALSE;
            if (temp < 0) return QSP_FALSE;
            /* var value */
            if (!qspSkipLines(&ind, strsCount, 1)) return QSP_FALSE;
        }
        /* indices count */
        if (!qspGetIntValueAndSkipLine(&groupsCount, &ind, strs, strsCount)) return QSP_FALSE;
        if (groupsCount < 0) return QSP_FALSE;
        /* indices */
        for (j = 0; j < groupsCount; ++j)
        {
            /* value index */
            if (!qspGetIntValueAndSkipLine(&temp, &ind, strs, strsCount)) return QSP_FALSE;
            if (temp < 0 || temp >= varValuesCount) return QSP_FALSE;
            /* text value */
            if (!qspSkipLines(&ind, strsCount, 1)) return QSP_FALSE;
        }
    }
    return (ind == strsCount - 1); /* the last line is always empty */
}

QSP_BOOL qspOpenGameStatus(void *data, int dataSize)
{
    QSPString *strs, locName, gameString;
    int i, j, ind, type, count, varInd, varsCount, valsCount;
    gameString = qspStringFromFileData(data, dataSize, QSP_TRUE);
    count = qspSplitStr(gameString, QSP_STATIC_STR(QSP_STRSDELIM), &strs);
    qspFreeString(gameString);
    if (!qspCheckGameStatus(strs, count))
    {
        qspSetError(QSP_ERR_CANTLOADFILE);
        qspFreeStrs(strs, count);
        return QSP_FALSE;
    }
    ++qspRefreshCount;
    ++qspFullRefreshCount;
    qspMemClear(QSP_FALSE);
    qspResetTime(qspDeCodeGetIntVal(strs[3]));
    qspCurSelAction = qspDeCodeGetIntVal(strs[4]);
    qspCurSelObject = qspDeCodeGetIntVal(strs[5]);
    qspViewPath = qspCodeDeCode(strs[6], QSP_FALSE);
    qspCurInput = qspCodeDeCode(strs[7], QSP_FALSE);
    qspCurDesc = qspCodeDeCode(strs[8], QSP_FALSE);
    qspCurVars = qspCodeDeCode(strs[9], QSP_FALSE);
    locName = qspCodeDeCode(strs[10], QSP_FALSE);
    qspCurIsShowActs = qspDeCodeGetIntVal(strs[11]) != 0;
    qspCurIsShowObjs = qspDeCodeGetIntVal(strs[12]) != 0;
    qspCurIsShowVars = qspDeCodeGetIntVal(strs[13]) != 0;
    qspCurIsShowInput = qspDeCodeGetIntVal(strs[14]) != 0;
    qspTimerInterval = qspDeCodeGetIntVal(strs[15]);
    qspPLFilesCount = qspDeCodeGetIntVal(strs[16]);
    ind = 17;
    for (i = 0; i < qspPLFilesCount; ++i)
        qspPLFiles[i] = qspCodeDeCode(strs[ind++], QSP_FALSE);
    qspCurIncFilesCount = qspDeCodeGetIntVal(strs[ind++]);
    for (i = 0; i < qspCurIncFilesCount; ++i)
        qspCurIncFiles[i] = qspCodeDeCode(strs[ind++], QSP_FALSE);
    qspCurActionsCount = qspDeCodeGetIntVal(strs[ind++]);
    for (i = 0; i < qspCurActionsCount; ++i)
    {
        qspCurActions[i].Image = qspCodeDeCode(strs[ind++], QSP_FALSE);
        qspCurActions[i].Desc = qspCodeDeCode(strs[ind++], QSP_FALSE);
        valsCount = qspCurActions[i].OnPressLinesCount = qspDeCodeGetIntVal(strs[ind++]);
        if (valsCount)
        {
            qspCurActions[i].OnPressLines = (QSPLineOfCode *)malloc(valsCount * sizeof(QSPLineOfCode));
            for (j = 0; j < valsCount; ++j)
            {
                qspInitLineOfCode(qspCurActions[i].OnPressLines + j, qspCodeDeCode(strs[ind++], QSP_FALSE), 0);
                qspCurActions[i].OnPressLines[j].LineNum = qspDeCodeGetIntVal(strs[ind++]);
            }
        }
        else
            qspCurActions[i].OnPressLines = 0;
        qspCurActions[i].Location = qspDeCodeGetIntVal(strs[ind++]);
        qspCurActions[i].ActIndex = qspDeCodeGetIntVal(strs[ind++]);
    }
    qspCurObjectsCount = qspDeCodeGetIntVal(strs[ind++]);
    for (i = 0; i < qspCurObjectsCount; ++i)
    {
        qspCurObjects[i].Image = qspCodeDeCode(strs[ind++], QSP_FALSE);
        qspCurObjects[i].Desc = qspCodeDeCode(strs[ind++], QSP_FALSE);
    }
    varsCount = qspDeCodeGetIntVal(strs[ind++]);
    for (i = 0; i < varsCount; ++i)
    {
        varInd = qspDeCodeGetIntVal(strs[ind++]);
        qspVars[varInd].Name = qspCodeDeCode(strs[ind++], QSP_FALSE);
        valsCount = qspDeCodeGetIntVal(strs[ind++]);
        if (valsCount)
        {
            qspVars[varInd].ValsCount = valsCount;
            qspVars[varInd].Values = (QSPVariant *)malloc(valsCount * sizeof(QSPVariant));
            for (j = 0; j < valsCount; ++j)
            {
                type = qspDeCodeGetIntVal(strs[ind++]);
                if (QSP_ISSTR(qspVars[varInd].Values[j].Type = type))
                    QSP_STR(qspVars[varInd].Values[j]) = qspCodeDeCode(strs[ind], QSP_FALSE);
                else
                    QSP_NUM(qspVars[varInd].Values[j]) = qspDeCodeGetIntVal(strs[ind]);
                ++ind;
            }
        }
        valsCount = qspDeCodeGetIntVal(strs[ind++]);
        if (valsCount)
        {
            qspVars[varInd].IndsBufSize = qspVars[varInd].IndsCount = valsCount;
            qspVars[varInd].Indices = (QSPVarIndex *)malloc(valsCount * sizeof(QSPVarIndex));
            for (j = 0; j < valsCount; ++j)
            {
                qspVars[varInd].Indices[j].Index = qspDeCodeGetIntVal(strs[ind++]);
                qspVars[varInd].Indices[j].Str = qspCodeDeCode(strs[ind++], QSP_FALSE);
            }
        }
    }
    qspFreeStrs(strs, count);
    qspIsMainDescChanged = qspIsVarsDescChanged = qspIsObjectsChanged = qspIsActionsChanged = QSP_TRUE;
    qspOpenIncludes();
    qspCurLoc = qspLocIndex(locName);
    qspFreeString(locName);
    if (qspErrorNum) return QSP_FALSE;
    qspCallShowWindow(QSP_WIN_ACTS, qspCurIsShowActs);
    qspCallShowWindow(QSP_WIN_OBJS, qspCurIsShowObjs);
    qspCallShowWindow(QSP_WIN_VARS, qspCurIsShowVars);
    qspCallShowWindow(QSP_WIN_INPUT, qspCurIsShowInput);
    qspCallSetInputStrText(qspCurInput);
    qspCallShowPicture(qspViewPath);
    qspPlayPLFiles();
    qspCallSetTimer(qspTimerInterval);
    qspExecLocByVarNameWithArgs(QSP_STATIC_STR(QSP_FMT("ONGLOAD")), 0, 0);
    return QSP_TRUE;
}

QSP_BOOL qspStatementOpenQst(QSPVariant *args, QSP_TINYINT count, QSPString *jumpTo, QSP_TINYINT extArg)
{
    int oldRefreshCount;
    switch (extArg)
    {
    case qspStatOpenQst:
        if (qspIsAnyString(QSP_STR(args[0])))
        {
            oldRefreshCount = qspRefreshCount;
            qspCallOpenGame(QSP_STR(args[0]), QSP_TRUE);
            if (qspRefreshCount != oldRefreshCount || qspErrorNum) return QSP_FALSE;
            qspNewGame(QSP_FALSE);
        }
        break;
    case qspStatIncLib:
        qspIncludeFile(QSP_STR(args[0]));
        break;
    }
    return QSP_FALSE;
}

QSP_BOOL qspStatementOpenGame(QSPVariant *args, QSP_TINYINT count, QSPString *jumpTo, QSP_TINYINT extArg)
{
    if (count && qspIsAnyString(QSP_STR(args[0])))
        qspCallOpenGameStatus(QSP_STR(args[0]));
    else
        qspCallOpenGameStatus(qspNullString);
    return QSP_FALSE;
}

QSP_BOOL qspStatementSaveGame(QSPVariant *args, QSP_TINYINT count, QSPString *jumpTo, QSP_TINYINT extArg)
{
    if (count && qspIsAnyString(QSP_STR(args[0])))
        qspCallSaveGameStatus(QSP_STR(args[0]));
    else
        qspCallSaveGameStatus(qspNullString);
    return QSP_FALSE;
}
