#include "Modifier.h"


using namespace Games::Rulesets::Modifiers;


Modifier::Modifier() :RegisterType("Modifier")
{
}

string Modifier::GetDescription()
{
	return description;
}
