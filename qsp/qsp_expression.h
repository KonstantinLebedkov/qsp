#pragma once

#include<array>
#include"qsp_variant.h"

#define QSP_OPMAXARGS 20
#define QSP_STACKSIZE 30
#define QSP_MAXITEMS 200

class qsp_expression
{
	//имхо, как раз тут имеет смысл пользоваться массивами - они в стеке а не в кипе. так оно пошустрее будет, да.
public:
	std::array<char, QSP_STACKSIZE> opStack;
	std::array<char, QSP_STACKSIZE> argStack;
	bool PushOpCode(int* opSp, char opCode);// пушит в массивы: в стек операторов - код оператора. в стек аргументов - минимальное их количество (если код оператора менее 25) либо ноль (если код 25 и более)
};

class qsp_CompiledExpression
{
	//TODO: у меня вариант - содержит строку. тоесть, он всё одно пойнтерный. а значит, на него надо звать деструктор. а значит, нет никакой проблемы перейти здесь к динамическому массиву, тоесть от array к variant
	//TODO: раз уж здесь всё одно пойнтерные свойства есть - то прописать деструктор обязательно необходимо. иначе ж оно потечет памятью!
public:
	int itemsCount;
	std::array<qsp_variant, QSP_MAXITEMS>compValues;
	std::array<char, QSP_MAXITEMS> compOpCodes, compArgsCounts;
	bool Append(char opCode, char argsCount, qsp_variant v);
	qsp_variant* Value(int valueIndex);
private:
	int SkipValue(int valueIndex);
	qsp_variant* ArgumentValue(int ArgIndex, char ArgType); // based on qspArgumentValue, gives qsp_variant calculated by index zand formatted as type
};

