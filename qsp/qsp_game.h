#pragma once

#include"qsp_world.h"
#include "qsp_strings.h"

#define Game qsp_game::GameHandler()

class qsp_game
{
    qsp_game() {};
    qsp_game(const qsp_game& root) = delete;
    qsp_game& operator=(const qsp_game& root) = delete;
    Strings CurIncFiles; //вектор строк с именами файлов (библотек игры?)
public:
    static qsp_game& GameHandler();
    int QstCRC;
    int CurIncLocsCount; //счетчик включенных в игру локаций. (нафига он нужен, если есть у класса World свойство Locs с методом size() ?)
    void ClearIncludes(bool isFirst); //некая чистка . импортировано, но посмотреть насколько актуально взависимости от применений.
};

