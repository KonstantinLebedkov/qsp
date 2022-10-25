#include "qsp_game.h"

void qsp_game::ClearIncludes(bool isFirst)
{
    int i, count;
    if (!isFirst)
    {//TODO: ������ - ������ �� ������� ������������� �����. � ������ ������ ������� ��� ������� ��� ����������, ��� � ������� ���� ����.
        //TODO: ������. ������ �������� ���� ���� ���������� ��������� �������, ����� ������� ����� �������.
        CurIncFiles.ClearStrings();
        if (CurIncLocsCount)
        {
            count = World.LocsCount() - CurIncLocsCount;
            //�� ������� ����� ��������� ������� � ����� � � ����(������ �������) ������� ���.... �� ���-�� ����.
            World.CreateWorld(count, count);
            //� ����� ���� ������� �������.
            qspPrepareLocs();
            //� � ����� ������������� ����� ��� ���� ������, ���� �������.
        }
    }
    CurIncLocsCount = 0;
}
