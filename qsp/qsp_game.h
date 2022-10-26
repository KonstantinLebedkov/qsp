#pragma once

#include"qsp_world.h"
#include "qsp_strings.h"

#define Game qsp_game::GameHandler()

class qsp_game
{
    qsp_game() {};
    qsp_game(const qsp_game& root) = delete;
    qsp_game& operator=(const qsp_game& root) = delete;
    Strings CurIncFiles; //������ ����� � ������� ������ (�������� ����?)
public:
    static qsp_game& GameHandler();
    int QstCRC;
    int CurIncLocsCount; //������� ���������� � ���� �������. (������ �� �����, ���� ���� � ������ World �������� Locs � ������� size() ?)
    void ClearIncludes(bool isFirst); //����� ������ . �������������, �� ���������� ��������� ��������� ������������ �� ����������.
};

