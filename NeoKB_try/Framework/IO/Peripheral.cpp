#include "Peripheral.h"

#include "../../Util/Log.h"

using namespace Framework::IO;
using namespace Util;


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
	LOG(LogLevel::Debug) << "Peripheral::PushOutputMessage() : get output message [" << outputMessage->ToString() << "]";
	unique_lock<mutex> uLock(outputMessageMutex);
	outputMessages.push_back(outputMessage);
	return 0;
}

int Peripheral::PourOutOutputMessages(vector<OutputMessage*>* pourOutTo)
{

	if(outputMessages.size() > 0)
		LOG(LogLevel::Depricated) << "Peripheral::PourOutOutputMessages() : pour all messages to board.";

	unique_lock<mutex> uLock(outputMessageMutex);
	pourOutTo->insert(pourOutTo->end(), outputMessages.begin(), outputMessages.end());
	outputMessages.clear();
	return 0;
}

