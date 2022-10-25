#pragma once

#include"qsp_world.h"
#include "qsp_strings.h"

class qsp_game
{
    //TODO: it must be singletone
    Strings CurIncFiles;
public:
    int qspQstCRC;
    int CurIncLocsCount;
    void ClearIncludes(bool isFirst);
};

