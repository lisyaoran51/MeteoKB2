#include "SheetmusicManager.h"

#include "../../Framework/Host/GameHost.h"
#include "ResourceStoreWorkingSm.h"


using namespace Games::Sheetmusics;
using namespace std;
using namespace Framework::Host;



SheetmusicStore * SmManager::createSmStore(function<DatabaseContext*()> gContext)
{
	return new SheetmusicStore(gContext);

	// TODO: �o���٭n�g�@�qstore.BeatmapSetAdded += s => BeatmapSetAdded?.Invoke(s);
	// ����C�U���@���q�A�N�|�hstore�̭��[�J�o���q�����
}

SmManager::SmManager(): RegisterType("SmManager")
{
	smInfos = new vector<SmInfo*>();
	rulesetInfos = new vector<RulesetInfo*>();
	// TODO: �q��Ʈw���[�J�C��ruleset info? ���Ӧbsm manager�~����ʥ[
}

SmManager::SmManager(Storage * s, function<DatabaseContext*()> gContext, RulesetStore * rStore, GameHost * gHost): RegisterType("SmManager")
{
	smInfos = new vector<SmInfo*>();

	// �o������ӥi�H�����F�A�{�b�令�qdatabase��
	rulesetInfos = new vector<RulesetInfo*>();
	// !!!�o�@�q�O�A�妺���A�H��n�令�q�ɮ�Ūruleset���
	rulesetInfos->push_back(new RulesetInfo("MeteorRuleset", 1));

	getContext = gContext;

	smStore = createSmStore(getContext);

	fileStore = new FileStore(gContext, s);

	// �o�䮳�쪺Storage�|�O�bFiles��Ƨ��U�A�]���[�Jfile store���ɭԷ|���L�[�@�h��Ƨ�Files
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

	// TODO : �e�Xinitializing�T��

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
	// �o�ӬO�s���A�H��n��sm�s��store��
	//return smStore->GetSheetmusics();

	// �o�ӬO�ª�
	return smInfos;
}

WorkingSm * SmManager::GetWorkingSm(SmInfo * s)
{
	//return new WorkingSm(s);
	return new ResourceStoreWorkingSm(fileStore->GetStore(), s);
}

vector<SmInfo*>* SmManager::import(FileReader & fileReader)
{
	// osu���g�k�O�@��beatmap�|���@��set�A�^�Ǥ@��set��istore���A���O�ڭ̪��]�p�S���ݭnset�A�@�Ӵ��N�O�@�Ӵ��A�@�Ӹ�Ƨ������ӳ��|���W�L�@�Ӵ�
	SmInfo* smInfo = importToStorage(fileStore, smStore, fileReader);

	// TODO: �b�ק�db���ɭ����ӭn��lock mutex
	smStore->Add(smInfo);
	
}

SmInfo * SmManager::importToStorage(FileStore * fStore, SheetmusicStore * sStore, FileReader & fileReader)
{
	vector<string>* smNames;

	smNames = fileReader.WhereEndWith(".sm");

	for (int i = 0; i < smNames->size(); i++) {

		LOG(LogLevel::Finer) << "vector<SmInfo*>* SmManager::importToStorage(FileReader&) : Getting stream of [" << smNames->at(i) << "].";

		ifstream* stream = fileReader.GetStream(smNames->at(i));

		// �C�@�ӭn�Ϊ�decoder�|�b�{���}�l���ɭԵ��U
		LOG(LogLevel::Finer) << "vector<SmInfo*>* SmManager::importToStorage(FileReader&) : Getting decoder of [" << smNames->at(i) << "].";
		SmDecoder* smDecoder = SmDecoder::GetDecoder(stream);



		LOG(LogLevel::Finer) << "vector<SmInfo*>* SmManager::importToStorage(FileReader&) : Decode [" << smNames->at(i) << "] ...";
		Sm<Event>* sm = smDecoder->Decode(stream);
		sm->GetSmInfo()->fileName = smNames->at(i);

		// TODO: ��o�q���ꪺcode��n�A���T���k���Ӥ��O�qfilereader��sm set info�A�n�htrace osu���g�k
		sm->GetSmInfo()->smSetInfo = fileReader.GetSmSetInfo();
		sm->GetSmInfo()->fileInfo = new FileInfo{
			sm->GetSmInfo()->id,
			fileReader.GetPath(),
			0
		};

		fStore->AddFile(sm->GetSmInfo()->fileInfo);

		// �n�b�o��[�J�p�G�٨S�o�Ӵ��A�N�g�J�s��
		

		// �g��o�� �����D����ruleset---> �b�إ�sm  manager�ɤ�ʧ�ruleset info�[�J
		// ���: ���osu�쥻���g�k�A�hdatabase��
		RulesetInfo* rulesetInfo = rulesetStore->GetRulesetInfo(sm->GetSmInfo()->rulesetId);


		/* ��ӧאּ��databaseŪruleset info�F

		// �o���A�妺�Adecoder�ѥX�Ӫ�ruleset id�۰ʳ]��1�A�M��sm manager��ruleset�۰ʥ[�JMeteorRuleset��id�O1
		// ����n�令�h�ɮ�Ū
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
			// TODO: �Q���~
		}

		smInfos->push_back(sm->GetSmInfo());


	}

	// TODO: �����Ʈw��n�H��A�o��n�令�U���o���Asminfos�令�\�b��Ʈw��
	//SmInfo* smInfo = smInfos->at(0);
	//delete smInfos;

	return smInfos->at(0);
}
