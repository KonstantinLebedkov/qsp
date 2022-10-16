#pragma once
#include "qsp_string.h"
class qsp_variant
{
public:
    union
    {
        qsp_string * Str;
        int * Num;
    } Val;
    char Type;
    qsp_variant(char Type = 0) :Type(Type) { if (Type == 0) Val.Num = new int(0); else if (Type == 1)Val.Str = new qsp_string(); };
    qsp_variant(char Type, int val) : Type(Type) { if (Type == 0) Val.Num = new int(val); else if (Type == 1) Val.Str = new qsp_string(std::to_wstring(val));}
    qsp_variant(char Type, qsp_string val) : Type(Type) { if (Type == 0) Val.Num = new int(std::stoi(val.data())); else if (Type == 1) Val.Str = new qsp_string(val); }//TODO: std::stoi can trow exception! than there must be additional checking for possibility of convertion. and, if possible - stoi, if impossible - 0
    bool IsAnyString() { if (Type == 1) return Val.Str->IsAnyString(); else return false; };
    ~qsp_variant() { if (Type==0) delete Val.Num; else if (Type==1) delete Val.Str; };
    //TODO: proverit sootvetstvie Type v sravnenii s originalnim QSPVariant.
};

