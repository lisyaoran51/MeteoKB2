#include "CommunicationRequest.h"


using namespace Framework::IO::Communications;


long long CommunicationRequest::getElapsedMicroseconds()
{
	requestCurrentTime = system_clock::now();

	return duration_cast<microseconds>(requestCurrentTime - requestStartTime).count();
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
	/* 不是scene就不能用request的callback */
	if (dynamic_cast<Scene*>(callableObject) == nullptr)
		return -1;

	/* 紀錄callback scene */
	if (callbackScene == nullptr)
		callbackScene = dynamic_cast<Scene*>(callableObject);

	/* 不允許由不同scene註冊，會當作error */
	if (callbackScene != dynamic_cast<Scene*>(callableObject))
		return -1;


	onSuccess.Add(callableObject, callback, name);
	return 0;
}

int CommunicationRequest::AddOnFailed(MtoObject * callableObject, function<int()> callback, string name)
{
	/* 不是scene就不能用request的callback */
	if (dynamic_cast<Scene*>(callableObject) == nullptr)
		return -1;

	/* 紀錄callback scene */
	if (callbackScene == nullptr)
		callbackScene = dynamic_cast<Scene*>(callableObject);

	/* 不允許由不同scene註冊，會當作error */
	if (callbackScene != dynamic_cast<Scene*>(callableObject))
		return -1;

	onFailed.Add(callableObject, callback, name);
	return 0;
}

int CommunicationRequest::AddOnCancelled(MtoObject * callableObject, function<int()> callback, string name)
{
	/* 不是scene就不能用request的callback */
	if (dynamic_cast<Scene*>(callableObject) == nullptr)
		return -1;

	/* 紀錄callback scene */
	if (callbackScene == nullptr)
		callbackScene = dynamic_cast<Scene*>(callableObject);

	/* 不允許由不同scene註冊，會當作error */
	if (callbackScene != dynamic_cast<Scene*>(callableObject))
		return -1;

	onCancelled.Add(callableObject, callback, name);
	return 0;
}

int CommunicationRequest::requestTimeStart()
{
	requestStartTime = system_clock::now();

	return 0;
}
