#include "qsp_action.h"

qsp_action::qsp_action(std::vector<qsp_variant*> args, QSPLineOfCode* code, int start, int end)
{
    if (args.size() == 2) if (args[1]->IsAnyString()) Image = new qsp_string(*args[1]->Val.Str);
    Desc = new qsp_string(*args[0]->Val.Str);
    qspCopyPrepLines(OnPressLines, code, start, end); //это копи-преп-лайнз какое-то сложное. не читал, надо будет разбираться.
    OnPressLinesCount = end - start;
    Location = qspRealCurLoc; //global var is in errors handler. what is wrong? it is not need in anything else?
    ActIndex = qspRealActIndex;
};