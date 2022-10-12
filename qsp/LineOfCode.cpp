#include "LineOfCode.h"
#include "text.h"
#include "statements.h"

QSPString QSPLineOfCode::qspGetLineLabel(QSPString str)
// ������� ��������� ����� �� ������, ���� ��� ���������� � ���������. ������� �����/����� ��������� ���������, ����� ������ � ������� ���������� ����������. ������� � ������ ��� �� ����������. ��������� ��������� � ���������
{
	std::size_t LabelPos = str.find(':');
	if (LabelPos != QSPString::npos)
	{
		std::size_t delimPos = str.find('&');
		QSPString result = str.substr(++LabelPos, (delimPos != QSPString::npos) ? delimPos - LabelPos : str.size() - LabelPos);
		qspDelSpc(&result);
		qspUpperStr(&result);
		return result;
	}
	return QSPString();
}

char QSPLineOfCode::qspGetStatCode(QSPString s, QSP_CHAR** pos){
	int i, strLen, nameLen;
	QSPStatName* name;
	if (s.empty()) return qspStatUnknown;
	if (*s.begin() == QSP_LABEL[0]) return qspStatLabel;
	if (*s.begin() == QSP_COMMENT[0]) return qspStatComment;
	/* ------------------------------------------------------------------ */
	strLen = s.length();
	for (i = 0; i < QSP_STATSLEVELS; ++i)
	{
		name = (QSPStatName*)bsearch(&s, qspStatsNames[i], qspStatsNamesCounts[i], sizeof(QSPStatName), qspStatStringCompare);
		if (name)
		{
			nameLen = qspStrLen(name->Name);
			if (nameLen == strLen || (nameLen < strLen && qspIsInClass(s.Str[nameLen], QSP_CHAR_DELIM)))
			{
				*pos = s.Str + nameLen;
				return name->Code;
			}
		}
	}
	return qspStatUnknown;
}

QSPLineOfCode::QSPLineOfCode(QSPString str, int lineNum) {
	Str = new QSPString (str); //TODO: � ����� ����� ������, ���� �������� �������� �� ����� � �� ������?
	LineNum = lineNum;
	StatsCount = 0;
	Stats = 0;
	int count = 0; //����������, ��� ������� ������, ��...
	QSP_CHAR * temp, * nextPos, * elsePos, * delimPos = 0, * paramPos = 0;
	char statCode = qspGetStatCode(str, &paramPos);//���������� ���� � ���-����, �� �� ������
	if (!qspIsEmpty(str) && statCode != qspStatComment) {

	}
	if (count == 1 && line->Stats[0].Stat == qspStatElse && statCode == qspStatIf &&
		*(line->Str.Str + line->Stats[0].ParamPos) != QSP_COLONDELIM[0]) {

	}
	else
	{

	}
	Stats[count].Stat = statCode;
	if (paramPos) {

	}
	Stats[count].ParamPos = (int)(str.Str - line->Str.Str);
	if (delimPos) {

	}
	else
	{

	}
	switch (line->Stats[0].Stat) {

	}
	Label = qspGetLineLabel(Str);
}
