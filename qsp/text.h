/* Copyright (C) 2001-2020 Valeriy Argunov (byte AT qsp DOT org) */
/* Copyright (C) 2022 Konstantin Lebedkov*/
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

#include "declarations.h"
#include "coding.h"

#define QSP_STRSDELIM QSP_FMT("\r\n")
#define QSP_LSUBEX QSP_FMT("<<")
#define QSP_RSUBEX QSP_FMT(">>")

    /* Frequently used classes of characters */
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

extern QSPString qspNullString;
//TODO: ����� �������, ��� ����-������ ���-�� �������� � ���������. � ������ ������, ����� ��� ������� � ���������� ��������?

    /* External functions */
QSP_CHAR *qspStringToC(QSPString s);
void qspAddText(QSPString *dest, QSPString val, QSP_BOOL isCreate);
QSP_BOOL qspClearText(QSPString *s);
QSP_CHAR *qspInStrRChars(QSPString str, QSP_CHAR *chars);
QSPString qspJoinStrs(QSPString *s, int count, QSPString delim);
int qspSplitStr(QSPString str, QSPString delim, QSPString **res);
void qspCopyStrs(QSPString **dest, QSPString *src, int start, int end);
void qspFreeStrs(QSPString *strs, int count);
QSP_BOOL qspIsNumber(QSPString s);
int qspStrToNum(QSPString s, QSP_BOOL *isValid);
QSPString qspNumToStr(QSP_CHAR *buf, int val);
QSP_CHAR *qspDelimPos(QSPString txt, QSP_CHAR ch);
QSP_CHAR *qspStrPos(QSPString txt, QSPString str, QSP_BOOL isIsolated);
QSPString qspReplaceText(QSPString txt, QSPString searchTxt, QSPString repTxt);
QSPString qspFormatText(QSPString txt, QSP_BOOL canReturnSelf);
int qspToWLower(int);
int qspToWUpper(int);


/*INLINE QSPString qspStringFromLen(QSP_CHAR* s, int len)
//TODO: ������ ����-������ �� ��������� �� ������ ������ � �� ����������.
//� �� ��������, ����� ���� ���������� �� ������ ���-�������, � �������� ��� ��, ����! � ��� �� ���� ����� �� ������ ������, � �� �� ������?
{
    QSPString string;
    string.Str = s;
    string.End = s + len;
    return string;
}*/

INLINE QSPString qspStringFromString(QSPString s, int maxLen)
//TODO: ������ ����-������ �� ������ ������, ������� �� ��� ������ � ��������. ��� ���� ����� ��������, ���������� ������� ������. �������, ��� ���� ���-�� ������ ������������ ������ � ������.
{
    int len = s.length();
    if (maxLen < len) s.erase(maxLen,len-maxLen);
    return s;
}

INLINE int qspStrLen(QSPString s)
// ��������� ����� ������
{
    return s.length();
}

INLINE QSP_BOOL qspIsEmpty(QSPString s)
// ��������, ��� ������ �� ������
{
    return s.empty();
}

INLINE void qspFreeString(QSPString s)
// ������� ����-������ �������� ������� �������. ����� ������ ���, ������.
{
    if (!s.empty()) s.clear();
}

    INLINE void qspUpdateText(QSPString *dest, QSPString val)
    {
        qspFreeString(*dest);
        qspAddText(dest, val, QSP_TRUE);
    }

    INLINE QSPString qspGetNewText(QSPString val)
    {
        QSPString string;
        qspAddText(&string, val, QSP_TRUE);
        return string;
    }

    INLINE QSP_BOOL qspIsInList(QSP_CHAR *list, QSP_CHAR ch)
    {
        while (*list)
            if (*list++ == ch) return QSP_TRUE;
        return QSP_FALSE;
    }

    INLINE QSP_BOOL qspIsInClass(QSP_CHAR ch, int charClass)
    {
        if (ch >= 128)
            return QSP_FALSE;

        return (qspAsciiClasses[ch] & charClass) != 0;
    }

    INLINE QSP_BOOL qspIsAnyInClass(QSPString str, int charClass)
    {
        QSPString::iterator pos = str.begin();
        while (pos < str.end())
        {
            if (qspIsInClass(*pos, charClass))
                return QSP_TRUE;
            ++pos;
        }
        return QSP_FALSE;
    }

    INLINE QSP_BOOL qspIsDigit(QSP_CHAR ch)
    {
        return (ch >= QSP_FMT('0') && ch <= QSP_FMT('9'));
    }

    INLINE void qspDelFirstSpc(QSPString *str)
    {
        size_t i = 0;
        while (str->at(i) == ' ' && i < str->size()) i++;
        if (i != 0) *str = str->substr(i);
    }

    INLINE void qspDelLastSpc(QSPString* str)
    {
        size_t i = str->size()-1;
        while (str->at(i) == ' ' && i > 0) i--;
        if (i != 0) *str = str->substr(0,i);
    }
    INLINE void qspDelSpc(QSPString* str)
    {
        qspDelFirstSpc(str);
        qspDelLastSpc(str);
    }


    INLINE QSP_BOOL qspIsAnyString(QSPString s)
    {
        qspDelFirstSpc(&s);
        return (!s.empty());
    }

    INLINE void qspLowerStr(QSPString *str)
    {
        QSPString::iterator pos = str->begin();
        while (pos < str->end())
        {
            *pos = (QSP_CHAR)QSP_CHRLWR(*pos);
            ++pos;
        }
    }

    inline void qspUpperStr(QSPString *str)
    {
        for (auto& c : *str) c = toupper(c);
    }

    INLINE int qspStrsNComp(QSPString str1, QSPString str2, int maxLen)
    {
        int delta = 0;
        QSPString::iterator pos1 = str1.begin(),  pos2 = str2.begin();
        while (maxLen-- && pos2 < str2.end() && pos1 < str1.end() && !(delta = (int)*pos1 - *pos2))
            ++pos1, ++pos2;
        return delta;
    }

    INLINE int qspStrsComp(QSPString str1, QSPString str2)
    {
        int delta = 0;
        QSPString::iterator pos1 = str1.begin(),  pos2 = str2.begin();
        while (pos2 < str2.end() && pos1 < str1.end() && !(delta = (int)*pos1 - *pos2))
            ++pos1, ++pos2;
        if (delta) return delta;
        return (pos1 == str1.end()) ? ((pos2 == str2.end()) ? 0 : -1) : 1;
    }

    INLINE QSPString::iterator qspStrChar(QSPString str, QSP_CHAR ch)
        //���������� �������� �� ������� ������, ����� �������� �� ����� ������
    {
        QSPString::iterator pos = str.begin();
        while (pos < str.end() && *pos != ch) ++pos;
        if (*pos == ch) return pos;
        return str.end();
    }

    INLINE QSPString::iterator qspStrPBrk(QSPString str, QSP_CHAR *strCharSet)
        //�������� �� ������, � ���� �� ������ - �� ����� ������
    {
        QSPString::iterator pos = str.begin();
        while (pos < str.end())
        {
            if (qspIsInList(strCharSet, *pos))
                return pos;
            ++pos;
        }
        return str.end();
    }

    INLINE QSPString::iterator spStrStr(QSPString str, QSPString strSearch)
    {
        QSPString::iterator s1, s2, pos = str.begin();
        while (pos < str.end())
        {
            s1 = pos;
            s2 = strSearch.begin();
            while (s1 < str.end() && s2 < strSearch.end() && !((int)*s1 - *s2))
                ++s1, ++s2;
            if (s2 == strSearch.end()) return pos;
            ++pos;
        }
        return str.end();
    }
