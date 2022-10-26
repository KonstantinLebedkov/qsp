#include "qsp_locNames.h"

void qsp_locNames::ClearTail(int start)
{
    for (auto i = end()-1; i >= begin() + start ; --i)
    {
        (*i).~qsp_locName();
        pop_back();
    }
}

void qsp_locNames::Sort()
{
    sort(begin(), end(), Compare);
}

bool qsp_locNames::Compare(qsp_locName a, qsp_locName b)
{
    return b.Name.compare(a.Name);
}
