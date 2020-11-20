#ifndef SHEETMUSIC_INFO_H
#define SHEETMUSIC_INFO_H

#include <string>
#include <fstream>
#include <vector>
#include "../../Util/TemplateConstraint.h"
#include "SheetmusicMetadata.h"
#include "SheetmusicDifficulty.h"
#include "SheetmusicSetInfo.h"
#include "../Ruleset/RulesetInfo.h"
#include "../../Framework/IO/FileInfo.h"



using namespace std;
using namespace Util;
using namespace Framework::IO;


namespace Games {
namespace Rulesets {

	class Ruleset;

}}


using namespace Games::Rulesets;

namespace Games {
namespace Sheetmusics {
	
	enum class SmHandType {
		/// <summary>
		/// 無選擇
		/// </summary>
		None,
		/// <summary>
		/// 僅能選擇使用手
		/// </summary>
		Hand,
		/// <summary>
		/// 僅能選擇難度
		/// </summary>
		Difficulty,
		/// <summary>
		/// 難度、左右手都能選擇
		/// </summary>
		All
	};

	struct SmInfo
	{

		int id;

		int smVersion;

		SmMetadata* metadata = nullptr;

		SmDifficulty* difficuty = nullptr;

		/// <summary>
		/// 歌曲的資料夾名稱
		/// </summary>
		string path;

		/// <summary>
		/// 歌曲的檔案名稱
		/// </summary>
		string fileName;

		// General
		int audioLeadIn;

		bool countdown = true;

		int version;

		/// <summary>
		/// 規則
		/// 1 : Meteor
		/// 2 : Instant
		/// </summary>
		int rulesetId = 0;

		RulesetInfo* rulesetInfo = nullptr;

		SmSetInfo* smSetInfo = nullptr;

		FileInfo* fileInfo = nullptr;

		/// <summary>
		/// 這份譜是否有標示小節
		/// </summary>
		bool hasSectionData = false;

		/// <summary>
		/// 這份譜是否有標示踏板，若無標示踏板就會auto sustain
		/// </summary>
		bool hasPedalData = false;

		SmHandType smHandType = SmHandType::None;

	};


}}



#endif