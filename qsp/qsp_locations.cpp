#include "qsp_locations.h"

void qsp_locations::ClearTail(int start)
{
    for (auto i = begin()+start; i < end(); ++i)
    { 
        (*i).~qsp_location();
    }
}
