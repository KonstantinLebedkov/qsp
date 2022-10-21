#pragma once

#include<vector>
#include "qsp_MathOperation.h"

class qsp_MathOperations : public std::vector <qsp_MathOperation*>
{
	//TODO: constructor by params
	//TODO: sorter by Code
	//TODO: destructor
public:
	void AddOperation(char opCode, char priority, qsp_Function func, char resType, char minArgs, char maxArgs, std::vector<char> args);
};

