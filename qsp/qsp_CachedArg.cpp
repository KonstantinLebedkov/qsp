#include "qsp_CachedArg.h"

qsp_CachedArg& qsp_CachedArg::operator=(const qsp_CachedArg& src)
{
    this->StartPos = src.StartPos;
    this->EndPos = src.EndPos;
    return *this;
}
