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

	class Modifier : public MtoObject{

	public:

		Modifier();

		virtual string GetDescription();

		virtual int SetValue(float value1, float value2);

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