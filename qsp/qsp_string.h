#pragma once
#include <string>

class qsp_string : public std::wstring
{
private:
    bool replaceEntryText(const qsp_string from, const qsp_string to);
public:
    //qsp_string() {};
    ~qsp_string(){};
    qsp_string replaceText(const qsp_string from, const qsp_string to);
    qsp_string replaceText(const std::wstring from, const std::wstring to);
    void replaceTextInSelf(const qsp_string from, const qsp_string to);
    void replaceTextInSelf(const std::wstring from, const std::wstring to);
};

