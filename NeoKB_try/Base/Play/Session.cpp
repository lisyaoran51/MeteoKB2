#include "Session.h"

using namespace Base::Play;
using namespace Base::Sheetmusics;

/* ���ӭn��Ū������Ū�W�h�M���A���O�o�{�W�h�M��������Ū(���Omto object)�A�ҥH���غc�l��
int Session::load()
{
	RulesetInfo* r = GetCache<RulesetInfo>("RulesetInfo");

	if (!r)
		throw runtime_error("int Session::load() : RulesetInfo not found in cache.");

	SmManager* s = GetCache<SmManager>("SmManager");

	if (!s)
		throw runtime_error("int MeteorPlayfield::load() : SmManager not found in cache.");

	return load(r, s);
}

int Session::load(RulesetInfo * r, SmManager * s)
{
	rulesetInfo = r;

	smManager = s;

	smInfo = smManager->GetSmInfos()->at(0);

	return 0;
}
*/

Session::Session(RulesetInfo* r, SmManager* s): RegisterType("Session"), ChildAddable()
{
	rulesetInfo = r;

	smManager = s;

	LOG(LogLevel::Finer) << "Session::Session(RulesetInfo*, SmManager*) : sm manager has [" << smManager->GetSmInfos()->size() << "] sm infos.";
	LOG(LogLevel::Finer) << "Session::Session(RulesetInfo*, SmManager*) : the first sm info is [" << smManager->GetSmInfos()->at(0)->fileName << "].";

	
	smInfos = smManager->GetSmInfos();
	smInfo = smManager->GetSmInfos()->at(0);	// �o�椧��n�R��

	//registerLoad(bind((int(Session::*)())&Session::load, this));
}

RulesetInfo * Session::GetRulesetInfo()
{
	return rulesetInfo;
}

// �o�椧��n�R��
WorkingSm * Session::GetWorkingSm()
{
	return smManager->GetWorkingSm(smInfo);
}

WorkingSm* Session::GetWorkingSm(string title){
	
	for(int i = 0; i < smInfos->size(); i++){
		
		if(smInfos->at(i)->metadata->Title == title)
			return smManager->GetWorkingSm(smInfos->at(i));
		
	}
	return NULL;
}
