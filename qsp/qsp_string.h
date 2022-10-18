#pragma once
#include <string>

class qsp_string : public std::wstring
{
private:
    bool replaceEntryText(const qsp_string from, const qsp_string to);
    bool replaceEntryText(const std::wstring from, const std::wstring to);
    void SkipSpaces();
public:
    //TODO: constructors required - default and assigning
    qsp_string() {};
    qsp_string(std::wstring str) { this->assign(str); };
    ~qsp_string(){};
    qsp_string replaceText(const qsp_string from, const qsp_string to);
    qsp_string replaceText(const std::wstring from, const std::wstring to);
    void replaceTextInSelf(const qsp_string from, const qsp_string to);
    void replaceTextInSelf(const std::wstring from, const std::wstring to);
    bool IsAnyString();//Skip first spaces if existing and check for existing something else
    int ToInteger();//convert to integer, to 0 if convertation impossible
    void UpperString();//convert UpCase
    void LowerString();//convert LowCase
    void DeleteSpaces(); //remove starting and ending spaces
    qsp_string& operator=(const std::wstring& source);
};

int qspStrsComp(qsp_string str1, qsp_string str2);
