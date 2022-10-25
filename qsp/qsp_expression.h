#pragma once

#include<array>
#include"qsp_variant.h"

#define QSP_OPMAXARGS 20
#define QSP_STACKSIZE 30
#define QSP_MAXITEMS 200

class qsp_expression
{
	//����, ��� ��� ��� ����� ����� ������������ ��������� - ��� � ����� � �� � ����. ��� ��� ��������� �����, ��.
public:
	std::array<char, QSP_STACKSIZE> opStack;
	std::array<char, QSP_STACKSIZE> argStack;
	bool PushOpCode(int* opSp, char opCode);// ����� � �������: � ���� ���������� - ��� ���������. � ���� ���������� - ����������� �� ���������� (���� ��� ��������� ����� 25) ���� ���� (���� ��� 25 � �����)
};

class qsp_CompiledExpression
{
	//TODO: � ���� ������� - �������� ������. ������, �� �� ���� ����������. � ������, �� ���� ���� ����� ����������. � ������, ��� ������� �������� ������� ����� � ������������� �������, ������ �� array � variant
	//TODO: ��� �� ����� �� ���� ���������� �������� ���� - �� ��������� ���������� ����������� ����������. ����� � ��� ������� �������!
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

