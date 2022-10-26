#pragma once

#include<algorithm>
#include<vector>
#include"qsp_locName.h"

class qsp_locNames : public std::vector<qsp_locName> //вектор указателей на локации, сортированный по именам локаций
{
	bool Compare(qsp_locName a, qsp_locName b);
public:
	void ClearTail(int start);
	void Clear() { ClearTail(0); };
	void Sort();
	~qsp_locNames() { ClearTail(0); clear(); };
};

