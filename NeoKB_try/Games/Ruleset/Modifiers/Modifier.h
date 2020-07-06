#ifndef MODIFIER_H
#define MODIFIER_H


#include <set>
#include <string>
#include "../../../Framework/Allocation/Hierachal/MtoObject.h"


using namespace std;
using namespace Framework::Allocation::Hierachal;


namespace Games {
namespace Rulesets{
namespace Modifiers{

	class Modifier : virtual public RegisterType{

	public:

		Modifier();

		virtual string GetDescription();

	protected:

		string description = "";

		set<string> incompitableMods;

		/// <summary>
		/// Returns if this mod is ranked.
		/// </summary>
		bool ranked = false;

	};

}}}



#endif