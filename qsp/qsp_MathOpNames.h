#pragma once

#include<vector>
#include "qsp_MathOpName.h"

class qsp_MathOpNames : public std::vector <qsp_MathOpName*>
{
	//TODO: make it singletone? bo, because it will be part of singletone fasad
	//but cleaner, destructor are required
public:
	void AddMathOp(char code, qsp_string name) { this->push_back(new qsp_MathOpName(code, name)); };
	char OperatorCode(qsp_string* expr);

};

