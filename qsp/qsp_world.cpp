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
{//TODO: ������ ��������, ����-�����, ���� �������� ������������� ����, ��! �� �� �� �����, � ��� ������������ ��������� ������. ������� �� ��������� ������� ���������.
    //TODO: �� ������ ��������� ����� �����, ���� �������� �� ������� ������� �����. ��� ����������/����������� ��������� ����.
    int i, j;
    Locs.ClearTail(start);
    LocsNames.ClearTail(start);
}

void qsp_world::PrepareLocs()
{
    int i;
    LocsNames.Clear();
    for (auto it : Locs) LocsNames.push_back(qsp_locName(it));
    //����� ���� ��������� ����������� �� �����.
    LocsNames.Sort();
}
