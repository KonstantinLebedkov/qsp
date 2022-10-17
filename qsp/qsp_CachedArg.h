#pragma once
class qsp_CachedArg
	//TODO: may be it will be better to use variant as body? it may contain string and int.. in my mind, that will requre more memory, but faster. because that type will be need in copy of string.
{
public:
	int StartPos;//postition of argument in string. //TODO: may be it will be better to use 
	int EndPos;//postition of argument in string. //TODO: may be it will be better to use
	qsp_CachedArg& operator=(const qsp_CachedArg& src);
	~qsp_CachedArg() {};
};

