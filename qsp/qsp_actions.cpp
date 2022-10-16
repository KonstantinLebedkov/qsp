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

void qsp_actions::AddAction(QSPVariant* args, QSPLineOfCode* code, int start, int end)
// вектор аргументов, каунт (набуа нужен? неясно. есть сайз вектора аргументов), код, начальный и конечный элемент которые надо взять из кода.
// еще вопрос - откель берутся переменные qspRealCurLoc и qspRealActIndex. это типа глобальные переменные, и получать их надо через вызов? или через ссыль на овнер? тогда нужен интерфейс к этому овнеру.
{
    qsp_action* act = new qsp_action();
    if (ActIndex((args[0]).Val.Str) != CurActs.end()) return; //Check vector of actions for action named like this. If found, stop adding
    if (CurActs.size() >= QSP_MAXACTIONS) { qspSetError(QSP_ERR_CANTADDACTION); return; }//Check amount of actions. if Limit achieved, stop and set error.
    act->Image = (args.size() == 2 && qspIsAnyString(args[1].Val.Str)) ? new qsp_string(args[1].Val.Str) : new qsp_string();
    act->Desc = new qsp_string(args[0].Val.Str);
    qspCopyPrepLines(&act->OnPressLines, code, start, end); //это копи-преп-лайнз какое-то сложное. не читал, надо будет разбираться.
    act->OnPressLinesCount = end - start;
    act->Location = qspRealCurLoc;
    act->ActIndex = qspRealActIndex;
    CurActs.push_back(act);
    IsActionsChanged = true;
}
