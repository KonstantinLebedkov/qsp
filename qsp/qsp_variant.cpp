#include "qsp_variant.h"
bool TypeConversionTable[QSP_TYPE_DEFINED_TYPES][QSP_TYPE_DEFINED_TYPES] =
{
    /*             NUMBER     STRING     CODE       TUPLE      VARREF */
    /* NUMBER */ { false, true,  true,  true,  true},
    /* STRING */ { true,  false, true,  true,  true},
    /* CODE */   { true,  false, false, true,  true},
    /* TUPLE */  { true,  false, true,  false, true},
    /* VARREF */ { true,  false, true,  true,  false},
};

void qsp_variant::ConvertTo(char type)
{
    if (Type != type && TypeConversionTable[Type][type])
    {
        if (type == QSP_TYPE_NUMBER)
        {
            if (Type == QSP_TYPE_STRING)
            {
                int num = Val.Str->ToInteger();
                Val.Str->~qsp_string();
                Val.Num = new int(num);
            }
        }
        else
        {
            if (Type == QSP_TYPE_NUMBER)
            {
                int v = *(Val.Num);
                delete Val.Num;
                Val.Str = new qsp_string(std::to_wstring(v));
            }
        }
        val->Type = type;
        FormatVariant();
    }
}

void qsp_variant::FormatVariant()
{
    switch (Type)
    {
        case QSP_TYPE_VARREF:
        {
            qsp_string temp = *(Val.Str);
            temp.DeleteSpaces();
            temp.UpperString();
            Val.Str->~qsp_string();
            Val.Str = new qsp_string(temp);
            break;
        }
    }
}
