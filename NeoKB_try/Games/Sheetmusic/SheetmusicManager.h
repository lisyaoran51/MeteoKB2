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




namespace Games {
namespace Sheetmusics {

	/// where T : effect_group
	/// 
	class SmManager
	{
		// ??不確定是否需要
		//static int copyTo(ifstream* from, ifstream* to);

	public:

		SmManager();

		SmManager(Storage* s, function<DatabaseContext*()> gContext, RulesetStore* rStore, GameHost* gHost);

		~SmManager();

		int RegisterRulesetInfo(RulesetInfo* r);

		int Import(vector<string>* paths);

		/// <summary>
		/// 從系統預設的路徑去抓檔案
		/// </summary>
		int ImportFromStable();

		vector<SmInfo*>* GetSmInfos();

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
		//vector<SmInfo*>* importToStorage(FileReader& fileReader); //目前沒有在存回storage所以不用

		// WorkingSm(sm_info_t* s);

		//virtual Sm<Event>* createSm() = 0;

	};


}}



#endif