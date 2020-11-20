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
		/// �L���
		/// </summary>
		None,
		/// <summary>
		/// �ȯ��ܨϥΤ�
		/// </summary>
		Hand,
		/// <summary>
		/// �ȯ�������
		/// </summary>
		Difficulty,
		/// <summary>
		/// ���סB���k�ⳣ����
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
		/// �q������Ƨ��W��
		/// </summary>
		string path;

		/// <summary>
		/// �q�����ɮצW��
		/// </summary>
		string fileName;

		// General
		int audioLeadIn;

		bool countdown = true;

		int version;

		/// <summary>
		/// �W�h
		/// 1 : Meteor
		/// 2 : Instant
		/// </summary>
		int rulesetId = 0;

		RulesetInfo* rulesetInfo = nullptr;

		SmSetInfo* smSetInfo = nullptr;

		FileInfo* fileInfo = nullptr;

		/// <summary>
		/// �o���ЬO�_���Хܤp�`
		/// </summary>
		bool hasSectionData = false;

		/// <summary>
		/// �o���ЬO�_���Хܽ�O�A�Y�L�Хܽ�O�N�|auto sustain
		/// </summary>
		bool hasPedalData = false;

		SmHandType smHandType = SmHandType::None;

	};


}}



#endif