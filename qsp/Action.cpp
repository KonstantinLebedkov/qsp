#include "Action.h"

Action::Action() {
    Image = new QSPString(""); 
    Desc = new QSPString(""); 
    OnPressLines = new QSPString("");
}

Action::Action(QSPString Des, QSPString Img, QSPString code) {
    Image = new QSPString(Des);
    Desc = new QSPString(Img);
    OnPressLines = new QSPString(code);
};
Action::~Action() {
    Image->~basic_string();
    Desc->~basic_string();
    OnPressLines->~basic_string();
}

void Action::Execute(int ind)
{
    if (ind >= 0 && ind < qspCurActionsCount)
    {
        int count;
        QSPString * code;
        QSPCurAct* act = qspCurActions + ind;
        /* switch the current state */
        qspRealCurLoc = Location;
        qspRealActIndex = ActIndex;
        count = OnPressLinesCount;
        qspCopyPrepLines(&code, OnPressLines, 0, count);
        qspExecCodeBlockWithLocals(code, 0, count, 1, 0);
        qspFreePrepLines(code, count);
    }
}