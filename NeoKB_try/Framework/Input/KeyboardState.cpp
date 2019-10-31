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

KeyboardState * KeyboardState::Clone()
{
	KeyboardState * cloned = new KeyboardState();

	cloned->SetLastState(lastState);
	cloned->GetPresses()->assign(presses.begin(), presses.end());


	return nullptr;
}

bool KeyboardState::Contain(InputKey key)
{
	throw logic_error("KeyboardState::Contain(): not implemented");
	return false;
}
