#include "MeteoPanelBoardV1.h"

#include <thread>
#include "../../Util/Log.h"
#include <cstring>


using namespace Desktop::Devices;
using namespace std;
using namespace Util;



vector<string> MeteoPanelBoardV1::split(string s, string pattern)
{
	string::size_type pos;
	vector<string> result;
	s += pattern;//扩展字符串以方便操作
	int size = s.size();

	for (int i = 0; i < size; i++)
	{
		pos = s.find(pattern, i);
		if (pos < size)
		{
			string split = s.substr(i, pos - i);
			result.push_back(split);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

string MeteoPanelBoardV1::trim(string s)
{
	if (s.length() == 0)
		return s;

	while (s.back() == ' ') {
		s = s.substr(0, s.size() - 1);
		if (s.length() == 0) {
			break;
		}
	}
	return s;
}


MeteoPanelBoardV1::MeteoPanelBoardV1(int address)
{

	i2cInterface = new Pi2c(address); // 不知為何arduino的位置是0x07? 是arduino的程式設定的

	panelState = new InputState();
	panelState->SetPanelState(new PanelState());

	keyboardState = new InputState();
	keyboardState->SetKeyboardState(new KeyboardState());

	thread t(&MeteoPanelBoardV1::work, this);
	t.detach();

}

InputState * MeteoPanelBoardV1::GetPanelState()
{
	if (panelState->GetPanelState()->CheckIsEmpty())
		return nullptr;

	InputState* returnValue = panelState;

	unique_lock<mutex> uLock(panelStateMutex);
	panelState = new InputState();
	panelState->SetPanelState(new PanelState());
	uLock.unlock();

	return returnValue;
}

InputState * MeteoPanelBoardV1::GetKeyboardState()
{
	if (keyboardState->GetKeyboardState()->CheckIsEmpty())
		return nullptr;

	InputState* returnValue = keyboardState;

	unique_lock<mutex> uLock(keyboardStateMutex);
	keyboardState = new InputState();
	keyboardState->SetKeyboardState(new KeyboardState());
	uLock.unlock();

	return returnValue;
}

int MeteoPanelBoardV1::SetSpeedKnobLight(int length)
{
	return 0;
}

int MeteoPanelBoardV1::SetSectionKnobLight(double roundTime)
{
	return 0;
}

int MeteoPanelBoardV1::SwitchLight(int button, bool isTurnOn)
{
	return 0;
}

int MeteoPanelBoardV1::PushI2cMessage(string iMessage)
{
	unique_lock<mutex> uLock(i2cMessageMutex);
	i2cMessages.push_back(iMessage);
	return 0;
}

int MeteoPanelBoardV1::work()
{
	while (!exitRequested) {
		readPanel();
		writePanel();
		this_thread::sleep_for(chrono::milliseconds(10));
	}
	return 0;
}

int MeteoPanelBoardV1::readPanel()
{
	char buffer[9];

	bool hasMessage = true;

	do {
		hasMessage = true;

		if (i2cInterface->i2cRead(buffer, 9) < 0)
			return -1;
		

		string i2cMessage = string(buffer);
		// trim whitespace
		i2cMessage = trim(i2cMessage);

		if (i2cMessage.length() == 0)
			hasMessage = false;
		else {

			LOG(LogLevel::Debug) << "MeteoPanelDevice::readFromDevice() : Get input from arduino [" << i2cMessage << "].";

			vector<string> splitMessage = split(i2cMessage, ",");
			InputKey key = InputKey::None;

			try {
				key = (InputKey)stoi(splitMessage[0]);
				stoi(splitMessage[1]);
			}
			catch (exception& e) {
				LOG(LogLevel::Error) << "MeteoPanelDevice::readFromDevice() : Get unknown input [" << i2cMessage << "].";
				continue;
			}

			if (int(key) < 500) { 
				pushKeyboardState(key, stoi(splitMessage[1]));
			}
			else { // pedal
				pushPanelState(key, stoi(splitMessage[1]));
			}

		}

	} while (hasMessage);


	return 0;
}

int MeteoPanelBoardV1::writePanel()
{

	if (i2cMessages.size() == 0)
		return -1;

	unique_lock<mutex> uLock(i2cMessageMutex);

	for (int i = 0; i < i2cMessages.size(); i++) {
		char *cstr = new char[i2cMessages[i].length() + 1];
		strcpy(cstr, i2cMessages[i].c_str());

		LOG(LogLevel::Debug) << "MeteoPanelBoardV1::writePanel() : write [" << cstr << "](" << i2cMessages[i].c_str() << ") to i2c.";

		i2cInterface->i2cWrite(cstr, sizeof(cstr));

		delete[] cstr;
	}
	i2cMessages.clear();

	return 0;
}

int MeteoPanelBoardV1::pushKeyboardState(InputKey key, int value)
{
	unique_lock<mutex> uLock(keyboardStateMutex);

	if (value >= 0) {
		if (!keyboardState->GetKeyboardState()->ContainPress(key))
			keyboardState->GetKeyboardState()->AddPress(make_pair(key, value));
	}
	else {
		if (!keyboardState->GetKeyboardState()->ContainUp(key))
			keyboardState->GetKeyboardState()->AddUp(key);
	}

	return 0;
}

int MeteoPanelBoardV1::pushPanelState(InputKey key, int value)
{
	unique_lock<mutex> uLock(panelStateMutex);

	if (int(key) < 1020) { // knob
		if (!panelState->GetPanelState()->ContainButton(key))
			panelState->GetPanelState()->AddButton(key);
	}
	else if(int(key) < 1030){ // slider
		if (!panelState->GetPanelState()->ContainKnob(key))
			panelState->GetPanelState()->AddKnob(make_pair(key, value));
	}
	else if (int(key) < 1500) { // plugin
		if (!panelState->GetPanelState()->ContainSlider(key))
			panelState->GetPanelState()->AddSlider(make_pair(key, value));
	}
	else {
		// plugin還沒寫
	}
		

	return 0;
}
