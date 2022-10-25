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
    { // ������� ������ ������ - � ������� ��� ���� � ����������, � ���-����� ��� ���������� ����.
    // ��� ��� ������ ���� ������� ������� �������. ������, ������ �� ��������, � ������ ������� � ���� ����� ����� �� �����.
        qspFreeString(qspLocsNames[i].Name); 
        qspFreeString(qspLocs[i].Name);
        qspFreeString(qspLocs[i].Desc);
        qspFreePrepLines(qspLocs[i].OnVisitLines, qspLocs[i].OnVisitLinesCount);
        //���� �� � ����� ������ ������������ ��� (�� ��������, ��� ��)
        for (j = 0; j < QSP_MAXACTIONS; ++j)
        {//���� ��, ������ ������ ��� ���� ���.
            if (qspLocs[i].Actions[j].Desc.Str)
            {
                qspFreeString(qspLocs[i].Actions[j].Image);
                qspFreeString(qspLocs[i].Actions[j].Desc);
                qspFreePrepLines(qspLocs[i].Actions[j].OnPressLines, qspLocs[i].Actions[j].OnPressLinesCount);
            }
        }
    }
    if (qspLocsCount != locsCount)
    {//�������� �����, ������� - ���� ���������� ��� �� ������������� ��������������, �������� ������ ������. � ��� ��� ������, ��������� � � �������� �������, ��� ��� � ���� �� �������
        qspLocsCount = locsCount;
        qspLocs = (QSPLocation*)realloc(qspLocs, qspLocsCount * sizeof(QSPLocation));
        qspLocsNames = (QSPLocName*)realloc(qspLocsNames, qspLocsCount * sizeof(QSPLocName));
    }//� �����, ������ ��� ���-�-��� ������������. � ������, �� ���-�-��� ��� � ����, � ������ ������� �� ������� � ������� �� ���� �������� ������, � ���� ����� �������� ���������.
    for (i = start; i < qspLocsCount; ++i)
    { //� ����� ������� ��������������� � ������ ���������� ���� "�������" � "��� ����" ������������ �������. 
        //� ������ - ������ ������ � "��� ����" � ������ ������ � ������ ������� ������ ����. � ��� ���(!!!) ���������� ���������� ���� ������ �������. �� ����������� ��� ��. � ������� �������� ��� ������� ������, �� ����� ����������� ���� ��� ���� �� ������� ��� 50 �������. ��� ��� ����� ������� � �������� ����� ��������.
        qspLocsNames[i].Name = qspNullString;
        for (j = 0; j < QSP_MAXACTIONS; ++j)
            qspLocs[i].Actions[j].Desc = qspNullString;
    }
}
