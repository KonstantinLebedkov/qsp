#include "qsp_actions.h"

qsp_actions::~qsp_actions()
{
	for (auto it : CurActs) 
		delete it; 
	CurActs.clear();
}

void qsp_actions::ClearActions()
{
    if (!CurActs.empty())
    {
        for (auto it : CurActs) it->~qsp_action();
        CurActs.clear();
        IsActionsChanged = true;
    }
    CurSelAction = CurActs.begin();
}

std::vector<qsp_action*>::iterator qsp_actions::ActIndex(qsp_string name)
{
    std::vector<qsp_action*>::iterator it;
    for (it = CurActs.begin(); it < CurActs.end(); it++) if (*(*it)->Desc == name) return it;
    return CurActs.end();
}

void qsp_actions::AddAction(std::vector<qsp_variant*> args, QSPLineOfCode* code, int start, int end)
// ������ ����������, ����� (����� �����? ������. ���� ���� ������� ����������), ���, ��������� � �������� ������� ������� ���� ����� �� ����.
// ��� ������ - ������ ������� ���������� qspRealCurLoc � qspRealActIndex. ��� ���� ���������� ����������, � �������� �� ���� ����� �����? ��� ����� ����� �� �����? ����� ����� ��������� � ����� ������.
{
    qsp_action* act = new qsp_action();
    if (ActIndex((args[0]).Val.Str) != CurActs.end()) return; //Check vector of actions for action named like this. If found, stop adding
    if (CurActs.size() >= QSP_MAXACTIONS) { qspSetError(QSP_ERR_CANTADDACTION); return; }//Check amount of actions. if Limit achieved, stop and set error.
    if(args.size() == 2) if (args[1]->IsAnyString()) act->Image->assign(*args[1]->Val.Str);
    act->Desc->assign(*args[0]->Val.Str);
    qspCopyPrepLines(&act->OnPressLines, code, start, end); //��� ����-����-����� �����-�� �������. �� �����, ���� ����� �����������.
    act->OnPressLinesCount = end - start;
    act->Location = qspRealCurLoc; //global var is in errors handler. what is wrong? it is not need in anything else?
    act->ActIndex = qspRealActIndex;
    CurActs.push_back(act);
    IsActionsChanged = true;
}
