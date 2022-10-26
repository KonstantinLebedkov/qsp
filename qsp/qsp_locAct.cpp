#include "qsp_locAct.h"

qsp_locAct::~qsp_locAct()
{
	Image.~qsp_string();
	Desc.~qsp_string();
	OnPressLines.~qsp_LinesOfCode();
}
