#ifndef SHEETMUSIC_MANAGER_H
#define SHEETMUSIC_MANAGER_H

#include <string>
#include <fstream>
#include <vector>
#include <functional>
#include "WorkingSheetmusic.h"
#include "../../Util/TemplateConstraint.h"
#include "SheetmusicInfo.h"
#include "Sheetmusic.h"
#include "IO/FileReader.h"
#include "Format/SmDecoder.h"
#include "../Ruleset/Ruleset.h"
#include "../Ruleset/RulesetInfo.h"
#include "../../Framework/IO/Storage.h"
#include "../Ruleset/RulesetStore.h"
#include "../../Framework/Database/DatabaseContext.h"
#include "../../Framework/Allocation/Hierachal/MtoObject.h"
#include "SheetmusicStore.h"
#include "../../Framework/IO/FileStore.h"


namespace Framework {
namespace Host{
	class GameHost;
}}


using namespace std;
using namespace Util;
using namespace Games::Sheetmusics::IO;
using namespace Games::Sheetmusics::Format;
using namespace Games::Rulesets;
using namespace Framework::IO;
using namespace Framework::Allocation::Hierachal;




namespace Games {
namespace Sheetmusics {

	/// where T : effect_group
	/// 
	class SmManager :public MtoObject
	{
		// ??���T�w�O�_�ݭn
		//static int copyTo(ifstream* from, ifstream* to);

		function<DatabaseContext*()> getContext;

		SheetmusicStore* smStore;

		FileStore* fileStore;

		Storage* storage;

		RulesetStore* rulesetStore;

		SheetmusicStore* createSmStore(function<DatabaseContext*()> gContext);

	public:

		SmManager();

		SmManager(Storage* s, function<DatabaseContext*()> gContext, RulesetStore* rStore, GameHost* gHost);

		~SmManager();

		int RegisterRulesetInfo(RulesetInfo* r);

		int Import(vector<string>* paths);

		/// <summary>
		/// �q�t�ιw�]�����|�h���ɮ�
		/// </summary>
		int ImportFromStable();

		/// <summary>
		/// �Χ��n�R��
		/// </summary>
		vector<SmInfo*>* GetSmInfos();

		WorkingSm* GetWorkingSm(SmInfo* s);

		/// <summary>
		/// �q�t�ιw�]�����|���Ҧ��x�s���e
		/// </summary>
		function<Storage*(void)> GetStableStorage;

	protected:

		vector<RulesetInfo*>* rulesetInfos;

		vector<SmInfo*>* smInfos;

		/// <summary>
		/// �|��o�Ǹ��v�̭����ɮץ���Ū�X�ӡA�åB��sm set�����@�դ@�ռ��СA�M����s��Y�Ӧa��(��Ʈw�άO?)
		/// ���O�ثe�S������o�ӳ����A�u����и�ƥ���i�h�@��list�̡A�ԫ�osu���@�k
		/// </summary>
		vector<SmInfo*>* import(FileReader& fileReader);

		/// <summary>
		/// �o�Ӫ��\����ܧ�sm�̭������ɸ��Ū�X�ӡA�̭���event�٤��ݭn�A�̭���event�n����C���}�l�ɦAŪ
		/// </summary>
		//vector<SmInfo*>* importToStorage(FileReader& fileReader); //�ثe�S���b�s�^storage�ҥH����

		// WorkingSm(sm_info_t* s);

		//virtual Sm<Event>* createSm() = 0;

	};


}}



#endif