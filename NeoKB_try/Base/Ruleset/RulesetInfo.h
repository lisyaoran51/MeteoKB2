#ifndef RULESET_INFO_H
#define RULESET_INFO_H


#include <string>


using namespace std;


namespace Base {
namespace Rulesets {
	
	class Ruleset;

	/// <summary>
	/// a game rule that designs how the game plays
	///	</summary>
	class RulesetInfo
	{

		int id;

		string name;

		string instantiateName;

	public:

		RulesetInfo(string n);

		Ruleset* CreateRuleset();

		int GetId();

		string GetTypeName();


	};



	

}}



#endif