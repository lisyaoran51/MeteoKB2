#ifndef MODIFIER_H
#define MODIFIER_H


#include <set>

using namespace std;


namespace Games {
namespace Rulesets{
namespace Modifiers{

	enum class ModifierType {
		Difficulty,
		Hand,
		Special
	};


	class Modifier {

	public:

		string GetName();

	protected:

		string name = "";

		string description = "";

		ModifierType modifierType = ModifierType::Special;

		set<string> incompitableMods;

		/// <summary>
		/// Returns if this mod is ranked.
		/// </summary>
		bool ranked = false;

	};

}}}



#endif