#pragma once

#include "qsp_Function.h"

class qsp_MathOperation
{
public:
    char Code;
    char Priority;
    char ResType;
    char MinArgsCount;
    char MaxArgsCount;
    std::vector <char> ArgsTypes;
    qsp_Function Func;
    qsp_MathOperation(char code, char priority, char resType, char minArgsCount, char maxArgsCount, qsp_Function func, std::vector<char>args) : Code(code), Priority(priority), ResType(resType), MinArgsCount(minArgsCount), MaxArgsCount(maxArgsCount), ArgsTypes(args) { Func = func; };
};

