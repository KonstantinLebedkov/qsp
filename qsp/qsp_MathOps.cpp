#include "qsp_MathOps.h"

void qsp_MathOps::AddOperation(char opCode, char priority, qsp_Function func, char resType, char minArgs, char maxArgs, std::vector<char>args)
{
    MathOperations.AddOperation(opCode, priority, func, resType, minArgs, maxArgs,args);
}

void qsp_MathOps::qspAddOpName(char opCode, qsp_string opName, int level)
{
    MathOpNames.at(level).push_back(new qsp_MathOpName(opCode, opName));
}
