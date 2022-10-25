#pragma once

#include<vector>
#include "qsp_MathOpName.h"

class qsp_MathOpNames : public std::vector <qsp_MathOpName*>
{
	//TODO: sorter

public:
	void AddMathOp(char code, qsp_string name) { this->push_back(new qsp_MathOpName(code, name)); };
	char OperatorCode(qsp_string* expr);
	qsp_MathOpNames::iterator GetCodeByName(qsp_string name);//TODO: realise it with search
	void Clean();
	~qsp_MathOpNames() { this->Clean(); };
};

