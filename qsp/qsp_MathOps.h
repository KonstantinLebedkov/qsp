#pragma once

#include "qsp_MathOpNames.h"
#include "qsp_Function.h"
#include "qsp_MathOperations.h"

class qsp_MathOps
{
	//TODO: make it singletone
//fields:
	std::vector<qsp_MathOpNames> MathOpNames;
	qsp_MathOperations MathOperations;
//methods
	void AddOperation(char opCode, char priority, qsp_Function func, char resType, char minArgs, char maxArgs, std::vector<char>args);
	void qspAddOpName(char opCode, qsp_string opName, int level);
	int qspMathOpsCompare(const void* opName1, const void* opName2);
	//use opName2->name.compare(opName1->name);
	int qspMathOpStringFullCompare(const void*, const void*);
	//as previous
	int qspMathOpStringCompare(const void*, const void*);
	//as previous, but only part of string sized by 2nd exemplar
	char qspFunctionOpCode(qsp_string funName);
	int qspGetNumber(qsp_string* expr);
	qsp_string qspGetName(qsp_string* expr);
	char qspOperatorOpCode(qsp_string* expr);
	qsp_string qspGetString(qsp_string* expr);
	qsp_string qspGetQString(qsp_string* expr);
	int qspFreeValue(int valueIndex, qsp_variant* compValues, QSP_TINYINT* compOpCodes, QSP_TINYINT* compArgsCounts);
	int qspSkipValue(int valueIndex, QSP_TINYINT* compArgsCounts);
public:
	void InitMath();
};

