#pragma once

#include<vector>
#include"qsp_locAct.h"
class qsp_locActs : public std::vector<qsp_locAct>
{
public:
	~qsp_locActs() { for (auto it : *this) it.~qsp_locAct(); };
};

