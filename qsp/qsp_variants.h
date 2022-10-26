#pragma once

#include<vector>
#include "qsp_variant.h"

//TODO: make template "qsp_vector" with optimized destructor of included objects

class qsp_variants :public std::vector<qsp_variant*>
{
public:
	void ClearVariants() { while (size()) { (*rbegin())->~qsp_variant(); pop_back(); } };
};

