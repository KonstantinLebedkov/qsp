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
#include <vector>
#include "codetools.h"
#include "variant.h"
#include "Action.h"

#define QSP_MAXACTIONS 50
//TODO: QSP_MAXACTIONS - � ����� �� ��� �����������? ��� ��� �����������? ��� ��� ����� �� ��? � ����� ����, � ������ ��� �� � �������?
class Actions {
    //TODO: singletone
    std::vector<Action*> List;
    int CurActionsCount;
    int CurSelAction;
    bool IsActionsChanged;
    bool IsShowActs;
    Action* ActIndex(QSPString);
public:
    Actions():CurActionsCount (0), CurSelAction (-1), IsActionsChanged (false), IsShowActs(true) {};
    //TODO: ��������� ����������� ��� ��������� ������
    ~Actions() {};
    //TODO: ��������� ���������� ��� ��������� ������ � ��������� ������� � �������.
    void ClearActions(bool); //TODO: ����������, ������� ������� �������. � ��������� �� ������ �������, �� ���������� �� ����� ���������� ������������.
    void AddAction(QSPVariant* args, char count, QSPString * code, int start, int end);
    //������� ���������� ������ � ���� �������.
    //TODO: ���� ����� ������ ���� �������� ���������� ���������.
    void ExecuteAction(int ind);
    QSPString qspGetAllActionsAsCode();

    void StatementSinglelineAddAct(QSPString * s, int statPos, int endPos);
    void StatementMultilineAddAct(QSPString * s, int lineInd, int endLine);
    bool StatementDelAct(QSPVariant* args, QSP_TINYINT count, QSPString* jumpTo, QSP_TINYINT extArg);
};

//TODO: ����� ���������� ����� ��� ���������� ��������.
//TODO: ������ ����� �������� � ���������� ������, ���� �����, ���������� ��� �����

extern QSPCurAct qspCurActions[QSP_MAXACTIONS];

    /* External functions */
    /* Statements */
