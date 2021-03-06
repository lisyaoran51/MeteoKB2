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

namespace Framework {
namespace IO{
namespace Api{

	class ApiAccess;

}}}



using namespace std;
using namespace Util;
using namespace Games::Sheetmusics::IO;
using namespace Games::Sheetmusics::Format;
using namespace Games::Rulesets;
using namespace Framework::IO;
using namespace Framework::IO::Api;
using namespace Framework::Allocation::Hierachal;




namespace Games {
namespace Sheetmusics {

	/// where T : effect_group
	/// 
	class SmManager :public MtoObject
	{
		// ??不確定是否需要
		//static int copyTo(ifstream* from, ifstream* to);

		function<DatabaseContext*()> getContext;

		SheetmusicStore* smStore = nullptr;

		FileStore* fileStore = nullptr;

		Storage* storage = nullptr;

		RulesetStore* rulesetStore = nullptr;

		ApiAccess* apiAccess = nullptr;

		SheetmusicStore* createSmStore(function<DatabaseContext*()> gContext);

		int setupApiAccess();

	public:

		SmManager();

		SmManager(Storage* s, function<DatabaseContext*()> gContext, RulesetStore* rStore, ApiAccess* aAccess, GameHost* gHost);

		~SmManager();

		int RegisterRulesetInfo(RulesetInfo* r);

		int Import(vector<string>* paths);

		/// <summary>
		/// 從系統預設的路徑去抓檔案
		/// </summary>
		int ImportFromStable();

		/// <summary>
		/// 用完要刪掉
		/// </summary>
		vector<SmInfo*>* GetSmInfos();

		/// <summary>
		/// 回傳值之後要改掉，可以參考osu
		/// </summary>
		int Download(SmInfo* s);

		WorkingSm* GetWorkingSm(SmInfo* s);

		/// <summary>
		/// 從系統預設的路徑抓到所有儲存內容
		/// </summary>
		function<Storage*(void)> GetStableStorage;

	protected:

		vector<RulesetInfo*>* rulesetInfos;

		vector<SmInfo*>* smInfos;

		/// <summary>
		/// 會把這些路競裡面的檔案全都讀出來，並且用sm set做成一組一組樂譜，然後佔存到某個地方(資料庫或是?)
		/// 但是目前沒有做到這個部分，只把樂譜資料先丟進去一個list裡，詳建osu的作法
		/// </summary>
		vector<SmInfo*>* import(FileReader& fileReader);

		/// <summary>
		/// 這個的功能指示把sm裡面的投檔資料讀出來，裡面的event還不需要，裡面的event要等到遊戲開始時再讀
		/// </summary>
		SmInfo* importToStorage(FileStore* fileStore, SheetmusicStore* smStore, FileReader& fileReader);

		//vector<SmInfo*>* importToStorage(FileReader& fileReader); //目前沒有在存回storage所以不用

		// WorkingSm(sm_info_t* s);

		//virtual Sm<Event>* createSm() = 0;

	};


}}



#endif