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



using namespace std;
using namespace Util;


namespace Base {
namespace Rulesets {

	class Ruleset;

}}


using namespace Base::Rulesets;

namespace Base {
namespace Sheetmusics {
	
	 
	struct SmInfo
	{

		int id;

		int smVersion;

		SmMetadata* metadata;

		SmDifficulty* difficuty;

		string fileName;

		int rulesetId;

		int audioLeadIn;

		int version;

		Ruleset* ruleset;

		RulesetInfo* rulesetInfo;

		SmSetInfo* smSetInfo;

	};


}}



#endif