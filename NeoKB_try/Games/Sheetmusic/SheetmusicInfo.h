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
		None,
		Hand,
		Difficulty,
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

		int rulesetId;

		RulesetInfo* rulesetInfo = nullptr;

		SmSetInfo* smSetInfo = nullptr;

		FileInfo* fileInfo = nullptr;

		bool section = false;

		SmHandType smHandType = SmHandType::None;

	};


}}



#endif