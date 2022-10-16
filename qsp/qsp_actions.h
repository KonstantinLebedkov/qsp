#pragma once
#include <vector>
#include "qsp_action.h"
#include "qsp_variant.h"
//TODO: actions have fields for store actID and locID, handled by error handler. may i need to made errorHandler class before qsp_actions class and give link to it?
// nu eto proshce chem soorujatj interfeisy
// v principe, errorHandler toje doljen byt singletone.
// i potomu, ego upominanija doljno bit dostatochno v samoi action, v konstruktore.
// i togda AddAction strukturno uproshcaetsia do: 1. proverka na yje dobavlennij action, 2. proverka na razmer. 3. push_back v vektor s vizovom konstruktora.
class qsp_actions
{
public:
    std::vector<qsp_action*> CurActs;
    // int CurActionsCount; //счетчик акшенов // не требуется, так как у вектора есть сайз.
    std::vector<qsp_action*>::iterator CurSelAction;
    bool IsActionsChanged;
    bool CurIsShowActs;
    qsp_actions():CurIsShowActs(true),IsActionsChanged(false),CurSelAction(CurActs.begin()) {};
    ~qsp_actions();
    void ClearActions();//bool flag isnt requred: if first, vector will be empty. if is not empty, it cant be first.
private:
    std::vector<qsp_action*>::iterator ActIndex(qsp_string name);//get iterator to vector's member named as requested. if unfound, returns end of vector.
public:
    void AddAction(std::vector<qsp_variant*> args, QSPLineOfCode* code, int start, int end);
    void Execute();
    qsp_string GetAllActionsAsCode();
    void StatementSinglelineAddAct();
    void StatementMultilineAddAct();
    bool StatementDelAct();
};

