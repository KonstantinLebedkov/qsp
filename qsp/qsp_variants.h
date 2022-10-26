#pragma once
#include<vector>
#include "qsp_variant.h"
class qsp_variants :public std::vector<qsp_variant*>
{
public:
	void ClearVariants() { while (size() > 0) { rbegin()->~qsp_variant; pop_back(); } };
};

