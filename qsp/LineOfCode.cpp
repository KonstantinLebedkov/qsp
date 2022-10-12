#include "LineOfCode.h"
#include "text.h"
#include "statements.h"

QSPString QSPLineOfCode::qspGetLineLabel(QSPString str)
// функци€ извлекает лейбл из строки, если она начинаетс€ с двоеточи€. пробелы перед/после двоеточи€ удал€ютс€, хвост начина с символа амперсанда отсекаетс€. пробелы с хвоста так же отсекаютс€. результат выводитс€ в апперкейс
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
	Str = new QSPString (str); //TODO: а набуа копи€ строки, если исходна€ получена по копии а не сцылке?
	LineNum = lineNum;
	StatsCount = 0;
	Stats = 0;
	int count = 0; //подозреваю, это счетчик статов, но...
	QSP_CHAR * temp, * nextPos, * elsePos, * delimPos = 0, * paramPos = 0;
	char statCode = qspGetStatCode(str, &paramPos);//гет—тат од есть в код-тулз, но не бечено
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
