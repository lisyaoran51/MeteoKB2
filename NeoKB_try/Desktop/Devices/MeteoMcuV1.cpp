﻿#include "MeteoMcuV1.h"

#include <thread>
#include "../../Util/Log.h"
#include <cstring>
#include <cmath>
#include "../../Framework/Threading/ThreadMaster.h"


using namespace Desktop::Devices;
using namespace std;
using namespace Util;
using namespace Framework::Threading;



vector<string> MeteoMcuV1::split(string s, string pattern)
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

string MeteoMcuV1::trim(string s)
{
	if (s.length() == 0)
		return s;

	while (s.back() == ' ' || s.back() == 0x0) {
		s = s.substr(0, s.size() - 1);
		if (s.length() == 0) {
			break;
		}
	}
	return s;
}


bool MeteoMcuV1::checkI2cMessageValid(InputKey iKey, int v)
{
	if (iKey > InputKey::None && iKey < InputKey::VK27_A1)
		return false;

	if (iKey > InputKey::VK37_C4 && iKey < InputKey::SustainPedal)
		return false;

	if (iKey > InputKey::ExpressionPedal && iKey < InputKey::Power)
		return false;

	if (iKey > InputKey::Speed && iKey < InputKey::RaiseOctave)
		return false;

	if (iKey > InputKey::Pause && iKey < InputKey::SectionKnob)
		return false;

	if (iKey > InputKey::SpeedKnob && iKey < InputKey::PianoVolumeSlider)
		return false;

	if (iKey > InputKey::MusicVolumeSlider && iKey < InputKey::Bluetooth)
		return false;

	if (iKey > InputKey::Bluetooth && iKey < InputKey::BluetoothPlugin)
		return false;

	if (iKey > InputKey::ExpressionPedalPlugin)
		return false;

	//if (v > 256 || v < -1)
	//	return false;
	if (v > 2000 || v < -1)
		return false;

	return true;
}

MeteoMcuV1::MeteoMcuV1(int address) : RegisterType("MeteoMcu")
{

	i2cInterface = new Pi2c(address); // 不知為何arduino的位置是0x07? 是arduino的程式設定的

	panelState = new InputState();
	panelState->SetPanelState(new PanelState());

	keyboardState = new InputState();
	keyboardState->SetKeyboardState(new KeyboardState());

	// 清除殘餘的i2c message
	char buffer[17] = { 0 };
	for (int i = 0; i < 10; i++)
		i2cInterface->i2cRead(buffer, 17);


	thisThread = new thread(&MeteoMcuV1::work, this);
	thisThread->detach();
	sleepTimeInMilliSecond = 0;

	ThreadMaster::GetInstance().AddSimpleThread(this);
}

InputState * MeteoMcuV1::GetPanelState()
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

InputState * MeteoMcuV1::GetKeyboardState()
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

int MeteoMcuV1::SetSpeedKnobLight(int length)
{
	return 0;
}

int MeteoMcuV1::SetSectionKnobLight(double roundTime)
{
	return 0;
}

int MeteoMcuV1::SwitchLight(int button, bool isTurnOn)
{
	return 0;
}

int MeteoMcuV1::PushI2cMessage(string iMessage)
{

	LOG(LogLevel::Depricated) << "MeteoMcuV1::PushI2cMessage() : push [" << iMessage << "]." << i2cMessages.size();

	unique_lock<mutex> uLock(i2cMessageMutex);
	i2cMessages.push_back(iMessage);
	return 0;
}

int MeteoMcuV1::work()
{
	while (!exitRequested) {
		LOG(LogLevel::Depricated) << "MeteoMcuV1::work() : output message size:" << i2cMessages.size();
		readPanel();
		writePanel();
		this_thread::sleep_for(chrono::milliseconds(sleepTimeInMilliSecond));
	}
	return 0;
}

int MeteoMcuV1::readPanel()
{
	char buffer[17] = {0};

	bool hasMessage = true;

	do {
		hasMessage = true;

		if (i2cInterface->i2cRead(buffer, 17) < 0)
			return -1;


		string i2cMessage = string(buffer);
		// trim whitespace
		i2cMessage = trim(i2cMessage);

		if (i2cMessage.length() == 0)
			hasMessage = false;
		else {

			if (i2cMessage[0] != 0x80)
				return -1;

			LOG(LogLevel::Depricated) << "MeteoMcuV1::readPanel() : Get input from mcu [" << i2cMessage << "].";

			vector<string> splitMessage;
			InputKey key = InputKey::None;

			try {
				splitMessage = split(i2cMessage, ",");

				if (splitMessage[0].length() > 5)
					throw runtime_error("MeteoMcuV1::readPanel() : Get unknown input.");

				key = static_cast<InputKey>(stoi(splitMessage[0].substr(1, splitMessage[0].length() - 1)));
				int value = stoi(splitMessage[1]);
				if (!checkI2cMessageValid(key, value)) {
					LOG(LogLevel::Error) << "MeteoMcuV1::readPanel() : Get unknown input [" << i2cMessage << "]." << (int)key << " " << value;
					throw out_of_range("MeteoMcuV1::readPanel() : Get unknown input.");
				}
			}
			catch (exception& e) {
				//LOG(LogLevel::Error) << "MeteoMcuV1::readPanel() : " << e.what();
				LOG(LogLevel::Error) << "MeteoMcuV1::readPanel() : Get unknown input [" << i2cMessage << "].";// with key[" << (int)key << "] and value[" << stoi(splitMessage[1]) << "].";
				//LOG(LogLevel::Error) << "MeteoMcuV1::readPanel() : convert to InputKey is [" << (int)static_cast<InputKey>(stoi(splitMessage[0].substr(1, splitMessage[0].length() - 1))) << "]";
				continue;
			}

			//continue;

			if (int(key) < 500) {
				if(stoi(splitMessage[1]) >= 2000)
					continue;

				if (stoi(splitMessage[1]) < 0) {
					pushKeyboardState(key, stoi(splitMessage[1]));
					continue;
				}


				int value = tan(((double)(2000 - stoi(splitMessage[1]))) / 2000.0 * 3.14159265358979323846 / 2.0) * 1.5;
				// tan(((double)(200 - (timerCount - pressedKeyTime[pressedNum])))/200.0*3.14159265358979323846/2.0)*2;
				LOG(LogLevel::Depricated) << "MeteoMcuV1::readPanel() : input is [" << stoi(splitMessage[1]) << "] this velocity is [" << value << "].";
				pushKeyboardState(key, value > 200 ? 200 : value);
			}
			else { // pedal
				pushPanelState(key, stoi(splitMessage[1]));
			}

		}

	} while (hasMessage);


	return 0;
}

int MeteoMcuV1::writePanel()
{
	
	if (!i2cMessageMutex.try_lock()) {
		LOG(LogLevel::Depricated) << "MeteoMcuV1::writePanel() : mutex locked";
		return 0;
	}
	

	//unique_lock<mutex> uLock(i2cMessageMutex);

	if (i2cMessages.size() == 0) {
		i2cMessageMutex.unlock();
		return -1;
	}
		

	LOG(LogLevel::Depricated) << "MeteoMcuV1::writePanel() : get i2c message";


	for (int i = 0; i < i2cMessages.size(); i++) {
		char *cstr = new char[i2cMessages[i].length() + 1];
		strcpy(cstr, i2cMessages[i].c_str());
		cstr[i2cMessages[i].length()] = '\0';

		LOG(LogLevel::Depricated) << "MeteoMcuV1::writePanel() : write [" << cstr << "](" << i2cMessages[i].c_str() << ") to i2c. last char is [" << (int)cstr[i2cMessages[i].length()] << "] at " << i2cMessages[i].length();

		try {
			i2cInterface->i2cWrite(cstr, i2cMessages[i].length() + 1);
		}
		catch (exception& e) {
			LOG(LogLevel::Error) << "MeteoMcuV1::writePanel() : write error [" << e.what() << "].";
		}

		LOG(LogLevel::Depricated) << "MeteoMcuV1::writePanel() : write over";

		delete[] cstr;
	}
	i2cMessages.clear();

	i2cMessageMutex.unlock();

	return 0;
}

int MeteoMcuV1::pushKeyboardState(InputKey key, int value)
{
	unique_lock<mutex> uLock(keyboardStateMutex);

	if (value >= 0) {
		if (!keyboardState->GetKeyboardState()->ContainPress(key))
			keyboardState->GetKeyboardState()->AddPress(make_pair(key, value));// > 127 ? 127 : value));
	}
	else {
		if (!keyboardState->GetKeyboardState()->ContainUp(key))
			keyboardState->GetKeyboardState()->AddUp(key);
	}

	return 0;
}

int MeteoMcuV1::pushPanelState(InputKey key, int value)
{
	unique_lock<mutex> uLock(panelStateMutex);

	if (int(key) < 1020) { // before knob is button
		if (!panelState->GetPanelState()->ContainButton(key)) {
			if (value == 1) {
				panelState->GetPanelState()->AddButton(pair<InputKey, bool>(key, true));
				LOG(LogLevel::Depricated) << "MeteoMcuV1::pushPanelState() : add button [" << int(key) << ",1].";
			}
			else {
				LOG(LogLevel::Depricated) << "MeteoMcuV1::pushPanelState() : add button [" << int(key) << ",0].";
				panelState->GetPanelState()->AddButton(pair<InputKey, bool>(key, false));
			}
			
		}
	}
	else if (int(key) < 1030) { // before slider is knob
		if (!panelState->GetPanelState()->ContainKnob(key))
			panelState->GetPanelState()->AddKnob(make_pair(key, value));
	}
	else if (int(key) < 1500) { // before plugin is slider
		if (!panelState->GetPanelState()->ContainSlider(key))
			panelState->GetPanelState()->AddSlider(make_pair(key, value));
	}
	else if (int(key) < 2000){
		if (value == 1) {
			panelState->GetPanelState()->AddButton(pair<InputKey, bool>(key, true));
			LOG(LogLevel::Depricated) << "MeteoMcuV1::pushPanelState() : add plugin [" << int(key) << ",1].";
		}
		else {
			LOG(LogLevel::Depricated) << "MeteoMcuV1::pushPanelState() : add plugin [" << int(key) << ",0].";
			panelState->GetPanelState()->AddButton(pair<InputKey, bool>(key, false));
		}
	}


	return 0;
}
