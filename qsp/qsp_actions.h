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
    void AddAction(std::vector<qsp_variant*> args, qsp_LinesOfCode* code, int start, int end);// adds one action, requres vector of varians (really, variant isnt needed, string will be enought), string with code, positions of start and end in this string.
    //TODO: go away of vector<variant> to vector<string>
    //go away from usege start and end (it isnt used now, but callers may be oriented to usage as sending full string and start from the middle, and end in the middle too.) that signs, it is requred substr usage somewhere.
    void Execute();
    qsp_string GetAllActionsAsCode();
    void StatementSinglelineAddAct();
    void StatementMultilineAddAct();
    bool StatementDelAct();
};

