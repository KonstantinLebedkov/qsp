#pragma once

#include "qsp_var.h"

class qsp_varsGroup : public std::vector<qsp_var*>
{
public:
	~qsp_varsGroup() { while (size()) { (*rbegin())->~qsp_var(); pop_back(); } };
};

