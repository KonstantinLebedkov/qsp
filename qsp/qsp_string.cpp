#include "qsp_string.h"

bool qsp_string::replaceEntryText(const qsp_string from, const qsp_string to) {
    size_t start_pos = this->find(from);
    if (start_pos == qsp_string::npos)
        return false;
    this->replace(start_pos, from.length(), to);
    return true;
};

qsp_string qsp_string::replaceText(const qsp_string from, const qsp_string to) {
    qsp_string result = new qsp_string(this);
    while (this->replaceEntryText(from, to) {};
    return result;
};
qsp_string qsp_string::replaceText(const std::wstring from, const std::wstring to) {
    qsp_string result = new qsp_string(this);
    while (this->replaceEntryText(from, to) {};
    return result;
};

void qsp_string::replaceTextInSelf(const qsp_string from, const qsp_string to) {
    while (this->replaceEntryText(from,to){}
};
void qsp_string::replaceTextInSelf(const std::wstring from, const std::wstring to) {
    while (this->replaceEntryText(from,to) {}
};
