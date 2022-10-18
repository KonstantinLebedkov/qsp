#pragma once
#include "qsp_string.h"

enum
{
    QSP_TYPE_NUMBER = 0,
    QSP_TYPE_STRING = 1,
    QSP_TYPE_CODE = 2,
    QSP_TYPE_TUPLE = 3,
    QSP_TYPE_VARREF = 4,
    QSP_TYPE_DEFINED_TYPES = 5, /* represents a number of defined values */
    QSP_TYPE_UNDEFINED = 64, /* not used for values, it has to be a string-based type */
};

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
    qsp_variant(char Type, qsp_string val) : Type(Type) { if (Type == 0) Val.Num = new int(std::stoi(val.data())); else if (Type == 1) Val.Str = new qsp_string(val); }
    bool IsAnyString() { if (Type == 1) return Val.Str->IsAnyString(); else return false; };
    ~qsp_variant() { if (Type==0) delete Val.Num; else if (Type==1) Val.Str->~qsp_string(); };
    void ConvertTo(char type);
    void FormatVariant(); //remove starting and ending spaces and convert upcase
};

