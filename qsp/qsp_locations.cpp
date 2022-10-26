#include "qsp_locations.h"

void qsp_locations::ClearTail(int start)
{
    for (auto it = end()-1; it >= begin() + start; --it)
    { 
        (*it)->~qsp_location();
        pop_back();
    }
}
