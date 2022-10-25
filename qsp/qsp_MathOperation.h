#pragma once

#include "qsp_Function.h"

class qsp_MathOperation
{
public:
    char Code;
    char Priority;
    char ResType;
    char minArgsCount;
    char maxArgsCount;
    std::vector <char> ArgsTypes;
    qsp_Function Func;
    qsp_MathOperation(char code, char priority, char resType, char minACount, char maxACount, qsp_Function func, std::vector<char>args) : Code(code), Priority(priority), ResType(resType), minArgsCount(minACount), maxArgsCount(maxACount), ArgsTypes(args) { Func = func; };
    ~qsp_MathOperation() {};
};

