#include "qsp_strings.h"

void qsp_strings::ClearStrings()
{
	for (auto s : *this)
	{
		s->~qsp_string();
	}
	this->clear();
}

qsp_strings::~Strings()
{
	ClearStrings();
}
