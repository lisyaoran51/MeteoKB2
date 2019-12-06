#include "KeyboardState.h"

using namespace std;
using namespace Framework::Input;


int KeyboardState::AddPress(pair<InputKey, int> press)
{
	presses.push_back(press);
	return 0;
}

int KeyboardState::AddUp(InputKey up)
{
	ups.push_back(up);
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

bool KeyboardState::ContainPress(InputKey key)
{
	for (int i = 0; i < GetPresses()->size(); i++)
		if (GetPresses()->at(i).first == key)
			return true;

	return false;
}

bool KeyboardState::ContainUp(InputKey key)
{
	for (int i = 0; i < GetUps()->size(); i++)
		if (GetUps()->at(i) == key)
			return true;
	return false;
}

bool KeyboardState::CheckIsEmpty()
{
	return presses.size() == 0 && ups.size() == 0;
}
