#include "qsp_world.h"

qsp_world& qsp_world::WorldHandler()
{
    static qsp_world theHandler;
    return theHandler;
}

int qsp_world::LocsCount()
{
    return Locs.size();
}

void qsp_world::CreateWorld(int start, int locsCount)
{
    int i, j;
    for (i = start; i < Locs.size(); ++i)
    { // сначала чистим строки - в локации это нейм и дескрипшен, в лок-нейме это собственно нейм.
    // вот тут чистку надо сделать методом вектора. причем, чистка не всеобщая, а хвоста начиная с локи номер Старт до конца.
        qspFreeString(qspLocsNames[i].Name); 
        qspFreeString(qspLocs[i].Name);
        qspFreeString(qspLocs[i].Desc);
        qspFreePrepLines(qspLocs[i].OnVisitLines, qspLocs[i].OnVisitLinesCount);
        //тако же в локах чистим пропарсенный код (по видимому, это он)
        for (j = 0; j < QSP_MAXACTIONS; ++j)
        {//тако же, чистим экшенз для этих лок.
            if (qspLocs[i].Actions[j].Desc.Str)
            {
                qspFreeString(qspLocs[i].Actions[j].Image);
                qspFreeString(qspLocs[i].Actions[j].Desc);
                qspFreePrepLines(qspLocs[i].Actions[j].OnPressLines, qspLocs[i].Actions[j].OnPressLinesCount);
            }
        }
    }
    if (qspLocsCount != locsCount)
    {//зачистив хвост, смотрим - если количество лок не соответствует затребованному, делается реалок памяти. и вот тут значит, поскольку я с вектором работаю, мне это в пень не впёрлось
        qspLocsCount = locsCount;
        qspLocs = (QSPLocation*)realloc(qspLocs, qspLocsCount * sizeof(QSPLocation));
        qspLocsNames = (QSPLocName*)realloc(qspLocsNames, qspLocsCount * sizeof(QSPLocName));
    }//к слову, массив тут так-и-так динамический. а значит, он так-и-так был в кипе, а потому переход от мессива к вектору не даст значимых потерь, а даже может вылиться приростом.
    for (i = start; i < qspLocsCount; ++i)
    { //и здесь набивка свежевыделенных в памяти переменных типа "локация" и "имя локи" минимальными данными. 
        //а именно - пустая строка в "имя локи" и пустые строки в массив акшенов каждой локи. и при том(!!!) пустышками набивается весь массив акшенов. он немаленький так то. а реально заюзаных там гораздо меньше, не особо представляю локу где были бы заюзаны все 50 акшенов. так что здесь переход к векторам будет выгодным.
        qspLocsNames[i].Name = qspNullString;
        for (j = 0; j < QSP_MAXACTIONS; ++j)
            qspLocs[i].Actions[j].Desc = qspNullString;
    }
}
