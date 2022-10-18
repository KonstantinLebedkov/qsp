#pragma once
#include<vector>
#include "qsp_variant.h"
class qsp_variants :public std::vector<qsp_variant*>
{
public:
	void ClearVariants() { for (qsp_variant* it : *this) it->~qsp_variant; this->clear(); };
};

