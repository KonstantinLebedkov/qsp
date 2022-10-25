#pragma once

#include "qsp_MathOpNames.h"
#include "qsp_Function.h"
#include "qsp_MathOperations.h"

#define MathOpNames qsp_MathOps::MathOpsHandler().mathOpNames
#define MathOperations qsp_MathOps::MathOpsHandler().mathOperations
#define MinArgsCount(x) MathOperations[x]->minArgsCount
#define MaxArgsCount(x) MathOperations[x]->maxArgsCount
#define FunctionOpCode(x) qsp_MathOps::MathOpsHandler().functionOpCode(x)

class qsp_MathOps
{
	qsp_MathOps() {};
	qsp_MathOps(const qsp_MathOps& root) = delete;
	qsp_MathOps& operator=(const qsp_MathOps& root) = delete;
//fields:
public: //temporaily its set to public
	std::vector<qsp_MathOpNames> mathOpNames;
	qsp_MathOperations mathOperations;
	char functionOpCode(String funName); //get Function's code by its name
private:
//methods
	void AddOperation(char opCode, char priority, qsp_Function func, char resType, char minArgs, char maxArgs, std::vector<char>args);
	void AddOpName(char opCode, String opName, int level);
	//do not use
	int qspMathOpsCompare(const void* opName1, const void* opName2);//use opName2->name.compare(opName1->name);
	//do not use
	int qspMathOpStringFullCompare(const void*, const void*);//as previous
	//do not use
	int qspMathOpStringCompare(const void*, const void*);//as previous, but only part of string sized by 2nd exemplar
	
	int GetNumber(String* expr);//TODO: move it to method of String class. Description: just convert string to int, and int can be non-negative only. if negative, its return max possible positive int, as protection from overflow
	String GetName(String* expr);//TODO: move it to method of String class.
	char OperatorOpCode(String* expr);//TODO: move it to method of String class?
	String GetQString(String* expr);//TODO: move it to method of String class.
	int FreeValue(int valueIndex, qsp_variant* compValues, std::vector<char> compOpCodes, std::vector<char> compArgsCounts);//DO NOT USE! it must be just destructor for qsp_variants (wich is vector of qsp_variant)
	int SkipValue(int valueIndex, char* compArgsCounts);// DO NOT USE IT! думаю, что это для разбора записей в стакане с записями. при некоем условии, часть записей скипается. и делается это рекурсивно.
	qsp_variant ArgumentValue(int valueIndex, char type, QSPVariant* compValues, char* compOpCodes, char* compArgsCounts); //второй аргумент зовет мембера от this->MathOperations[VI], третий, четвертый и пятый вышепришедшие массивы
	// и внутри себя зовет кусп-валюэ, который внутри зовет разные аргумент-валюэ.
	// других вызывающих точек аргумент-валюэ не имеет.
	// кусп-валюэ вызывается из стринг-> экспр-валлюэ, и только оттуда. других точек вызова не имеет.
	// таким образом, комп-валюэс, комп-оп-кодес и сомп-арг-каунтс должны быть просто частью одного объекта. динамического. который и создается в стринговом экпр-валюэ, дальнейшее идет его методами.
public:
	static qsp_MathOps& MathOpsHandler();
	void InitMath(); //TODO: realise it, copy from original code with conversion
};

