#include "qsp_location.h"

qsp_location::~qsp_location()
{
	Name.~qsp_string();
	Desc.~qsp_string();
	OnVisitLines.~qsp_LinesOfCode();
	Actions.~qsp_locActs();
}
