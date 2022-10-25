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
        String * Str;
        int * Num;
    } Val;
    char Type;
    qsp_variant(char Type = 0) :Type(Type) { if (Type) Val.Str = new qsp_string(); else Val.Num = new int(0); };
    qsp_variant(char Type, int val) : Type(Type) { if (Type) Val.Str = new qsp_string(std::to_wstring(val)); else Val.Num = new int(val);}
    qsp_variant(char Type, qsp_string val) : Type(Type) { if (Type) Val.Str = new qsp_string(val); else Val.Num = new int(std::stoi(val.data())); }
    qsp_variant(const qsp_variant &src) : Type(src.Type) {if (Type) Val.Str = new String(*src.Val.Str); else Val.Num = new int(*src.Val.Num); };
    bool IsAnyString() { if (Type>0) return Val.Str->IsAnyString(); else return false; };
    ~qsp_variant() { if (Type) Val.Str->~qsp_string(); else  delete Val.Num;};

    void ConvertTo(char type); //based on qspConvertVariantTo(...). original is bool, but always true. 
    void FormatVariant(); //remove starting and ending spaces and convert upcase
    bool CanConvertToNum(); //based on "qspIsCanConvertToNum"
    //TODO: CanConvert calls non-redesigned qspStrToNum(QSPString, bool) - redesign it
    bool IsNum() { return (Type == QSP_TYPE_NUMBER); };
    bool IsStr() { return (Type > QSP_TYPE_NUMBER); };
    bool IsDef() { return (Type != QSP_TYPE_UNDEFINED); };
    bool BaseType() { return (Type > QSP_TYPE_NUMBER); };
    //TODO:
    /*
    int qspAutoConvertCompare(QSPVariant *v1, QSPVariant *v2);
    void qspUpdateVariantValue(QSPVariant *dest, QSPVariant *src);
    */
};

