#pragma once

#include<vector>
#include"qsp_string.h"

#define Strings qsp_strings

class qsp_strings : public std::vector<String*>
{
public:
	void ClearStrings();
	~Strings();
};

