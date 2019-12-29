#include "Peripheral.h"

using namespace Framework::IO;

int Peripheral::SetDevice(Device * device)
{
	matchedDevice = device;
	return 0;
}

int Peripheral::PushInputState(InputState * inputState)
{
	inputStates.push_back(inputState);
	return 0;
}

int Peripheral::PushOutputMessage(OutputMessage * outputMessage)
{
	//¦n¹³­nthread safe
	unique_lock<mutex> uLock(outputMessageMutex);
	outputMessages.push_back(outputMessage);
	return 0;
}

int Peripheral::PourOutOutputMessages(vector<OutputMessage*>* pourOutTo)
{

	unique_lock<mutex> uLock(outputMessageMutex);
	pourOutTo->insert(pourOutTo->end(), outputMessages.begin(), outputMessages.end());
	outputMessages.clear();
	return 0;
}

