#pragma once

#include<algorithm>
#include<vector>
#include"qsp_locName.h"

class qsp_locNames : public std::vector<qsp_locName>
{
	bool Compare(qsp_locName a, qsp_locName b);
public:
	void ClearTail(int start);
	void Sort();
};

