#pragma once

#include "qsp_string.h"

class qsp_locName
{
public:
	//TODO: ��������, ����� ������ ���� �� ���, � ������� �� ���� ��� �������� �� ��� �� � �������.
	int Index;
	String Name;
	~qsp_locName() { Name.~qsp_string(); };
};

