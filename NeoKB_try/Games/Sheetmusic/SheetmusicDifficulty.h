#ifndef SHEETMUSIC_DIFFICULTY_H
#define SHEETMUSIC_DIFFICULTY_H

#include <string>
#include <fstream>
#include <vector>
#include "../../Util/TemplateConstraint.h"



using namespace std;
using namespace Util;

namespace Games {
namespace Sheetmusics {
	
	enum class  SmDifficultyHandType {
		None,
		Both,
		Left,
		Right
	};

	enum class SmDifficultyDifficulty {
		None,
		Easy,
		Medium,
		Hard
	};

	class SmDifficulty
	{

	public:

		SmDifficultyHandType HandType = SmDifficultyHandType::None;

		SmDifficultyDifficulty Difficulty = SmDifficultyDifficulty::None;

		/// <summary>
		/// ���U�t�רC���h���A16�N�O�@�����@�ӥ������
		/// </summary>
		float FallSpeed = 16;

	};


}}



#endif