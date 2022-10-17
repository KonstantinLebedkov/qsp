#pragma once
#include <vector>
#include "qsp_action.h"
class qsp_actions
{
public:
    std::vector<qsp_action*> CurActs;
    std::vector<qsp_action*>::iterator CurSelAction;
    bool IsActionsChanged;
    bool CurIsShowActs;
    qsp_actions():CurIsShowActs(true),IsActionsChanged(false),CurSelAction(CurActs.begin()) {};
    ~qsp_actions();
    void ClearActions();//bool flag isnt requred: if first, vector will be empty. if is not empty, it cant be first.
private:
    std::vector<qsp_action*>::iterator ActIndex(qsp_string name);//get iterator to vector's member named as requested. if unfound, returns end of vector.
public:
    void AddAction(std::vector<qsp_variant*> args, qsp_LinesOfCode* code, int start, int end);
    void Execute();
    qsp_string GetAllActionsAsCode();
    void StatementSinglelineAddAct();
    void StatementMultilineAddAct();
    bool StatementDelAct();
};

