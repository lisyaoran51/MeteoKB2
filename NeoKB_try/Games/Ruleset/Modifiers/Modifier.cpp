#include "Modifier.h"


using namespace Games::Rulesets::Modifiers;


Modifier::Modifier() :RegisterType("Modifier")
{
}

string Modifier::GetDescription()
{
	return description;
}

int Modifier::SetValue(int value1, int value2)
{
	return 0;
}
