#include <cwctype>
#include "qsp_string.h"

bool qsp_string::replaceEntryText(const qsp_string from, const qsp_string to) {
    size_t start_pos = this->find(from);
    if (start_pos == qsp_string::npos)
        return false;
    this->replace(start_pos, from.length(), to);
    return true;
};

bool qsp_string::replaceEntryText(const std::wstring from, const std::wstring to) {
    size_t start_pos = this->find(from);
    if (start_pos == qsp_string::npos)
        return false;
    this->replace(start_pos, from.length(), to);
    return true;
};

void qsp_string::SkipSpaces()
{
    while ((*begin() == *(L" ")) && begin() != end()) this->erase(0, 1);
};

qsp_string qsp_string::replaceText(const qsp_string from, const qsp_string to) {
    qsp_string result = qsp_string(*this);
    while (result.replaceEntryText(from, to)) {};
    return result;
};

qsp_string qsp_string::replaceText(const std::wstring from, const std::wstring to) {
    qsp_string result = qsp_string(*this);
    while (result.replaceEntryText(from, to)) {};
    return result;
};

void qsp_string::replaceTextInSelf(const qsp_string from, const qsp_string to) {
    while (this->replaceEntryText(from,to)){}
};

void qsp_string::replaceTextInSelf(const std::wstring from, const std::wstring to) {
    while (this->replaceEntryText(from, to)) {}
};

bool qsp_string::IsAnyString()
{
    //TODO: IMHO, it is not good to check for spaces here. it must be cleared from spaces during initial parsing and nomore else.
    //TODO: this method called for some variant fields. for example, in qsp.actions->AddAction. for union, it must be checked for field type before, another case may cause error.
    qsp_string str(*this);
    str.SkipSpaces();
    return str.empty();
}

int qsp_string::ToInteger()
{
    return std::stoi(*this);
}

void qsp_string::UpperString()
{
    for (auto it : *this) it = std::towupper(it);
}

void qsp_string::LowerString()
{
    for (auto it : *this) it = std::towlower(it);
}

void qsp_string::DeleteSpaces()
{
    qsp_string::iterator begin = this->begin();
    qsp_string::iterator end = this->end();
    while (begin < end && *begin != L' ') ++begin;
    while (begin < end && *(end - 1)!=L' ') --end;
    this->assign(this->substr(distance(this->begin(), begin), distance(begin, end)));
}

qsp_string& qsp_string::operator=(const std::wstring& source)
{
    this->assign(source);
    return *this;
};


int qspStrsComp(qsp_string str1, qsp_string str2)
{
    int delta = 0;
    std::wstring::iterator pos1 = str1.begin(), pos2 = str2.begin();
    while (pos2 < str2.end() && pos1 < str1.end() && !(delta = (int)(*pos1 - *pos2)))
        ++pos1, ++pos2;
    if (delta) return delta;
    return (pos1 == str1.end()) ? ((pos2 == str2.end()) ? 0 : -1) : 1;
}