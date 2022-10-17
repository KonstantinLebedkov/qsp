#include "qsp_LinesOfCode.h"

qsp_LinesOfCode& qsp_LinesOfCode::operator=(const qsp_LinesOfCode& src)
{
    if (!src.Lines.empty())
    {
        for (auto it : src.Lines)
        {
            Lines.push_back(new qsp_LineOfCode(*it));
        }
    }
}
