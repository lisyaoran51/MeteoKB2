#include "Modifier.h"


using namespace Games::Rulesets::Modifiers;


Modifier::Modifier() :RegisterType("Modifier")
{
}

string Modifier::GetDescription()
{
	return description;
}

int Modifier::SetValue(float value1, float value2)
{
	return 0;
}
