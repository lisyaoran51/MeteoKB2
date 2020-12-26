#include "CommunicationRequest.h"


using namespace Framework::IO::Communications;


long long CommunicationRequest::getElapsedMicroseconds()
{
	systemCurrentTime = system_clock::now();

	return duration_cast<microseconds>(systemCurrentTime - systemStartTime).count();
}

double CommunicationRequest::getElapsedSeconds()
{
	long long eMicroSeconds = getElapsedMicroseconds();
	return double(eMicroSeconds / 1000000) + double(eMicroSeconds % 1000000) * 0.000001;
}

int CommunicationRequest::AddCommunicationComponentOption(string componentName, deque<CommunicationRequest*>* componentRequestQueue)
{
	acceptedCommunicationComponentRequestQueues[componentName] = componentRequestQueue;

	return 0;
}

int CommunicationRequest::Fail(CommunicationRequestException & communicationRequestException)
{
	onFailed.Trigger();

	return fail(communicationRequestException);
}

int CommunicationRequest::AddOnSuccess(MtoObject * callableObject, function<int()> callback, string name)
{
	onSuccess.Add(callableObject, callback, name);
	return 0;
}

int CommunicationRequest::AddOnFailed(MtoObject * callableObject, function<int()> callback, string name)
{
	onFailed.Add(callableObject, callback, name);
	return 0;
}

int CommunicationRequest::AddOnCancelled(MtoObject * callableObject, function<int()> callback, string name)
{
	onCancelled.Add(callableObject, callback, name);
	return 0;
}
