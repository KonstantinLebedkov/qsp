#include "qsp_game.h"

qsp_game& qsp_game::GameHandler()
{
    static qsp_game theHandler;
    return theHandler;
}

void qsp_game::ClearIncludes(bool isFirst)
{
    int i, count;
    if (!isFirst)
    {
        CurIncFiles.ClearStrings();
        if (CurIncLocsCount)
        {
            count = World.LocsCount() - CurIncLocsCount;
            //�� ������� ����� ��������� ������� � ����� � � ����(������ �������) ������� ���.... �� ���-�� ����.
            World.CreateWorld(count, count); //������� ���� ������� ����� count
            World.PrepareLocs(); //��������� ������ ���� �������
            //� � ����� ������������� ����� ��� ���� ������, ���� �������.
        }
    }
    CurIncLocsCount = 0;
}
