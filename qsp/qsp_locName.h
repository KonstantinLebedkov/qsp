#pragma once

#include "qsp_string.h"
#include "qsp_location.h"

class qsp_locName
{
public:
	qsp_location * Loc;
	String Name;
	qsp_locName(qsp_location* loc) : Loc(loc), Name(loc->Name) {Name.UpperString() };
	~qsp_locName() { Name.~qsp_string(); };
};

