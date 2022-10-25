#include "qsp_MathOperations.h"

void qsp_MathOperations::AddOperation(char opCode, char priority, qsp_Function func, char resType, char minArgs, char maxArgs, std::vector<char>args)
{
	this->push_back(new qsp_MathOperation(opCode, priority, resType, minArgs, maxArgs, func, args));
}

void qsp_MathOperations::Clean()
{
	for (auto it : *this) 
		it->~qsp_MathOperation(); 
	this->clear();
}