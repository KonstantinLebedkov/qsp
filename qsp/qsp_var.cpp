#include "qsp_var.h"

void qsp_var::EmptyVar()
{
    int count;
    if (Values.size()) Values.ClearVariants();
    if (Indices.size()) Indices.clear();
    qspInitVarData(var);
}
