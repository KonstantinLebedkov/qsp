#pragma once

#include <vector>
#include "qsp_Statement.h"
#include "qsp_StatName.h"
#include "qsp_CachedStat.h"

class qsp_statements
{
public:
	static qsp_statements& StatementsHandler();
	qsp_Statement Stats[StatLast_Statement];
	//std::vector<qsp_Statement> Stats;
	qsp_StatName StatsNames[QSP_STATSLEVELS][QSP_MAXSTATSNAMES];
	//std::vector <std::vector <qsp_StatName>> StatsNames;
	int StatsNamesCounts[QSP_STATSLEVELS];
	//std::vector <int> StatsNamesCounts;
	//int StatMaxLen; //not found any usage of this var
	void Init();
	qsp_variants GetStatArgs(qsp_string s, qsp_CachedStat* stat); // full vector of variants with values extracted from string "s" by arguments of stat.
	//TODO: may be, GetStatArgs must be member of CachedStat?
	//bool qspExecCode(QSPLineOfCode* s, int startLine, int endLine, int codeOffset, QSPString* jumpTo);
	//bool qspExecCodeBlockWithLocals(QSPLineOfCode* s, int startLine, int endLine, int codeOffset, QSPString* jumpTo);
	//void qspExecStringAsCodeWithArgs(qsp_string s, qsp_variant* args, char count, int codeOffset, qsp_variant* res);
private:
	qsp_statements() {};
	qsp_statements(const qsp_statements& root) = delete;
	qsp_statements& operator=(const qsp_statements& root) = delete;
	void AddStatement(char statCode, qsp_STATEMENT func, char minArgs, char maxArgs, std::vector<char>args);
	void AddStatName(char statCode, qsp_string statName, int level);
	void AddStatName(char statCode, std::wstring statName, int level);
	int StatsCompare(const void*, const void*);
	static bool StatementImplicitStatement (qsp_variant *args, char count, qsp_string* jumpTo, char extArg); //судя по изменяемой в теле CurDesc - это должно быть методом в "Common.h"
	//TODO: переместить кудаследует.

	//TODO: еще вопрос: тут получается, что нет смысла делать инит таким какой он есть - потому как регать функции надо при инициализации соответствующих хендлеров.
	static bool StatementClear (qsp_variant* args, char count, qsp_string* jumpTo, char extArg);
	static bool StatementDynamic(qsp_variant* args, char count, qsp_string* jumpTo, char extArg);
	static bool StatementExec(qsp_variant* args, char count, qsp_string* jumpTo, char extArg);
	static bool StatementExit(qsp_variant* args, char count, qsp_string* jumpTo, char extArg);
	static bool StatementGoSub(qsp_variant* args, char count, qsp_string* jumpTo, char extArg);
	static bool StatementGoTo(qsp_variant* args, char count, qsp_string* jumpTo, char extArg);
	static bool StatementJump(qsp_variant* args, char count, qsp_string* jumpTo, char extArg);
	static bool StatementAddText(qsp_variant* args, char count, qsp_string* jumpTo, char extArg);
	static bool StatementMsg(qsp_variant* args, char count, qsp_string* jumpTo, char extArg);
};


