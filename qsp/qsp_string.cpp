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
}
bool qsp_string::IsAnyString()
{
    //TODO: IMHO, it is not good to check for spaces here. it must be cleared from spaces during initial parsing and nomore else.
    //TODO: this method called for some variant fields. for example, in qsp.actions->AddAction. for union, it must be checked for field type before, another case may cause error.
    qsp_string str(*this);
    str.SkipSpaces();
    return str.empty();
}
qsp_string& qsp_string::operator=(const std::wstring& source)
{
    this->append(source);
}
;
