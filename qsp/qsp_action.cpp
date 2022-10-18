#include "qsp_action.h"
#include "qsp_errors.h"

qsp_action::qsp_action(std::vector<qsp_variant*> args, qsp_LinesOfCode* code, int start, int end)
{
    if (args.size() == 2) if (args[1]->IsAnyString()) Image = new qsp_string(*args[1]->Val.Str);
    Desc = new qsp_string(*args[0]->Val.Str);
    OnPressLines = code;
    Location = qsp_errors::ErrorsHandler().RealCurLoc; //global var is in errors handler. what is wrong? it is not need in anything else?
    ActIndex = qsp_errors::ErrorsHandler().RealActIndex;
};

void qsp_action::Execute()
{
    qsp_errors::ErrorsHandler().RealCurLoc = Location;
    qsp_errors::ErrorsHandler().RealActIndex = ActIndex;
    //count = OnPressLinesCount;//OnPressLines в сущности обертка вектора. который имеет сайз. и потому сей счётчик нафих не надобен.
    qsp_LinesOfCode* code = OnPressLines;
    code.execute();    qspExecCodeBlockWithLocals(code, 0, count, 1, 0);
    code->~qsp_LinesOfCode();
};