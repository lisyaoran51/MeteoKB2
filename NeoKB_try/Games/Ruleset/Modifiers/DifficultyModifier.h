#ifndef DIFFICULTY_MODIFIER_H
#define DIFFICULTY_MODIFIER_H


#include "../../Sheetmusic/SheetmusicDifficulty.h"
#include "Modifier.h"


using namespace Games::Sheetmusics;


namespace Games {
namespace Rulesets{
namespace Modifiers{

	/// <summary>
	/// An interface for mods that make general adjustments to difficulty¡B¥ª¥k¤â.
	/// </summary>
	class DifficultyModifier: virtual public Modifier {

	public:

		DifficultyModifier();

		virtual int ApplyToDifficulty(SmDifficulty* smDifficulty) = 0;

	};


}}}





#endif