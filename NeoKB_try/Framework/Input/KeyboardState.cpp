#include "KeyboardState.h"

using namespace std;
using namespace Framework::Input;


int KeyboardState::AddPress(pair<InputKey, int> press)
{
	presses.push_back(press);
	return 0;
}

vector<pair<InputKey, int>>* KeyboardState::GetPresses()
{
	return &presses;
}

vector<InputKey>* KeyboardState::GetUps()
{
	return &ups;
}

KeyboardState * KeyboardState::Clone()
{
	KeyboardState * cloned = new KeyboardState();

	if(lastState != nullptr)
		cloned->SetLastState(lastState);
	cloned->GetPresses()->assign(presses.begin(), presses.end());
	cloned->GetUps()->assign(ups.begin(), ups.end());


	return cloned;
}

bool KeyboardState::Contain(InputKey key)
{
	throw logic_error("KeyboardState::Contain(): not implemented");
	return false;
}
