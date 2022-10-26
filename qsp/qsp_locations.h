#pragma once

#include"qsp_location.h"
class qsp_locations : public std::vector<qsp_location>
{
public:
	void ClearTail(int start);
};

