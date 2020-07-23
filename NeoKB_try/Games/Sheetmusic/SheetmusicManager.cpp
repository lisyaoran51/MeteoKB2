#include "SheetmusicManager.h"

#include "../../Framework/Host/GameHost.h"
#include "ResourceStoreWorkingSm.h"


using namespace Games::Sheetmusics;
using namespace std;
using namespace Framework::Host;



SheetmusicStore * SmManager::createSmStore(function<DatabaseContext*()> gContext)
{
	return new SheetmusicStore(gContext);

	// TODO: 這邊還要寫一段store.BeatmapSetAdded += s => BeatmapSetAdded?.Invoke(s);
	// 之後每下載一首歌，就會去store裡面加入這首歌的資料
}

SmManager::SmManager(): RegisterType("SmManager")
{
	smInfos = new vector<SmInfo*>();
	rulesetInfos = new vector<RulesetInfo*>();
	// TODO: 從資料庫中加入每個ruleset info? 應該在sm manager外面手動加
}

SmManager::SmManager(Storage * s, function<DatabaseContext*()> gContext, RulesetStore * rStore, GameHost * gHost): RegisterType("SmManager")
{
	smInfos = new vector<SmInfo*>();

	// 這兩行應該可以拿掉了，現在改成從database拿
	rulesetInfos = new vector<RulesetInfo*>();
	// !!!這一段是鮮血死的，以後要改成從檔案讀ruleset資料
	rulesetInfos->push_back(new RulesetInfo("MeteorRuleset", 1));

	getContext = gContext;

	smStore = createSmStore(getContext);

	fileStore = new FileStore(gContext, s);

	// 這邊拿到的Storage會是在Files資料夾下，因為加入file store的時候會幫他加一層資料夾Files
	storage = fileStore->GetStorage();

	rulesetStore = rStore;

	//ipc = new BeatmapIPCChannel(importHost, this); 

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

	// TODO : 送出initializing訊息

	for (int i = 0; i < paths->size(); i++) {
		FileReader fileReader(paths->at(i));
		import(fileReader);
	}

	return 0;
}

int SmManager::ImportFromStable()
{
	LOG(LogLevel::Info) << "SmManager::ImportFromStable(): Start Import.";
	Storage* stable = GetStableStorage();

	vector<string>* paths = stable->GetDirectories("Songs");
	paths->push_back(stable->GetUsableDirectoryPathFor("Songs"));

	LOG(LogLevel::Info) << "SmManager::ImportFromStable() : import paths ^^^" << [](vector<string>* p) {

		for (int i = 0; i < p->size(); i++) {
			LOG(LogLevel::Info) << "---------------------------- " << p->at(i);
		}

		return 0;
	}(paths);

	Import(paths);

	delete stable;
	delete paths;

	return 0;
}

vector<SmInfo*>* SmManager::GetSmInfos()
{
	// 這個是新的，以後要把sm存到store李
	//return smStore->GetSheetmusics();

	// 這個是舊的
	return smInfos;
}

WorkingSm * SmManager::GetWorkingSm(SmInfo * s)
{
	//return new WorkingSm(s);
	return new ResourceStoreWorkingSm(fileStore->GetStore(), s);
}

vector<SmInfo*>* SmManager::import(FileReader & fileReader)
{
	// osu的寫法是一個beatmap會有一組set，回傳一組set丟進store李，但是我們的設計沒有需要set，一個普就是一個普，一個資料夾裡應該部會有超過一個普
	SmInfo* smInfo = importToStorage(fileStore, smStore, fileReader);

	// TODO: 在修改db的時候應該要用lock mutex
	smStore->Add(smInfo);
	
}

SmInfo * SmManager::importToStorage(FileStore * fStore, SheetmusicStore * sStore, FileReader & fileReader)
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
		sm->GetSmInfo()->fileInfo = new FileInfo{
			sm->GetSmInfo()->id,
			fileReader.GetPath(),
			0
		};

		fStore->AddFile(sm->GetSmInfo()->fileInfo);

		// 要在這邊加入如果還沒這個普，就寫入新譜
		

		// 寫到這邊 不知道怎麼建ruleset---> 在建立sm  manager時手動把ruleset info加入
		// 後來: 改用osu原本的寫法，去database抓
		RulesetInfo* rulesetInfo = rulesetStore->GetRulesetInfo(sm->GetSmInfo()->rulesetId);


		/* 後來改為用database讀ruleset info了

		// 這邊鮮血死，decoder解出來的ruleset id自動設為1，然後sm manager的ruleset自動加入MeteorRuleset的id是1
		// 之後要改成去檔案讀
		for (int i = 0; i < rulesetInfos->size(); i++){
			if (rulesetInfos->at(i)->GetId() == sm->GetSmInfo()->rulesetId) {
				rulesetInfo = rulesetInfos->at(i);
				break;
			}
		}
		*/

		if (rulesetInfo)
			sm->GetSmInfo()->rulesetInfo = rulesetInfo;
		else {
			// TODO: 噴錯誤
		}

		smInfos->push_back(sm->GetSmInfo());


	}

	// TODO: 之後資料庫改好以後，這邊要改成下面這兩行，sminfos改成擺在資料庫裡
	//SmInfo* smInfo = smInfos->at(0);
	//delete smInfos;

	return smInfos->at(0);
}
