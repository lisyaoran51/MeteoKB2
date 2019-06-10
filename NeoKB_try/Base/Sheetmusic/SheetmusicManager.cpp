#include "SheetmusicManager.h"


using namespace Base::Sheetmusics;
using namespace std;

SmManager::SmManager()
{
	smInfos = new vector<SmInfo*>();
	rulesetInfos = new vector<RulesetInfo*>();
	// TODO: 從資料庫中加入每個ruleset info? 應該在sm manager外面手動加
}

SmManager::~SmManager()
{
	for (int i = 0; i < smInfos->size(); i++)
		delete smInfos->at(i);
	for (int i = 0; i < rulesetInfos->size(); i++)
		delete rulesetInfos->at(i);

	delete smInfos;
	delete rulesetInfos;
}

int SmManager::RegisterRulesetInfo(RulesetInfo * r)
{
	rulesetInfos->push_back(r);
	return 0;
}

int SmManager::Import(vector<string>* paths)
{
	for (int i = 0; i < paths->size(); i++) {
		FileReader fileReader(paths->at(i));
		import(fileReader);
	}

	return 0;
}

vector<SmInfo*>* SmManager::GetSmInfos()
{
	return smInfos;
}

WorkingSm * SmManager::GetWorkingSm(SmInfo * s)
{
	return new WorkingSm(s);
}

vector<SmInfo*>* SmManager::import(FileReader & fileReader)
{
	return importToStorage(fileReader);
}

vector<SmInfo*>* SmManager::importToStorage(FileReader & fileReader)
{
	vector<string>* smNames;

	smNames = fileReader.WhereEndWith(".sm");

	for (int i = 0; i < smNames->size(); i++) {

		LOG(LogLevel::Finer) << "vector<SmInfo*>* SmManager::importToStorage(FileReader&) : Getting stream of [" << smNames->at(i) << "].";

		ifstream* stream = fileReader.GetStream(smNames->at(i));

		// 每一個要用的decoder會在程式開始的時候註冊
		LOG(LogLevel::Finer) << "vector<SmInfo*>* SmManager::importToStorage(FileReader&) : Getting decoder of [" << smNames->at(i) << "].";
		SmDecoder* smDecoder = SmDecoder::GetDecoder(stream);



		LOG(LogLevel::Finer) << "vector<SmInfo*>* SmManager::importToStorage(FileReader&) : Decode [" << smNames->at(i) << "] ...";
		Sm<Event>* sm = smDecoder->Decode(stream);
		sm->GetSmInfo()->fileName = smNames->at(i);

		// TODO: 把這段佔實的code改好，正確做法應該不是從filereader拿sm set info，要去trace osu的寫法
		sm->GetSmInfo()->smSetInfo = fileReader.GetSmSetInfo();

		// 寫到這邊 不知道怎麼建ruleset---> 在建立sm  manager時手動把ruleset info加入
		RulesetInfo* rulesetInfo = NULL;

		for (int i = 0; i < rulesetInfos->size(); i++){
			if (rulesetInfos->at(i)->GetId() == sm->GetSmInfo()->rulesetId) {
				rulesetInfo = rulesetInfos->at(i);
				break;
			}
		}

		if(rulesetInfo)
			sm->GetSmInfo()->rulesetInfo = rulesetInfo;
		else{
			// TODO: 噴錯誤
		}

		smInfos->push_back(sm->GetSmInfo());
	}



	return smInfos;
}
