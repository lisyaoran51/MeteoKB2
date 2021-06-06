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

long long CommunicationRequest::getSectionElapsedMicroseconds()
{
	requestCurrentTime = system_clock::now();

	return duration_cast<microseconds>(requestCurrentTime - requestPointTime).count();
}

double CommunicationRequest::getSectionElapsedSeconds()
{
	long long eMicroSeconds = getSectionElapsedMicroseconds();
	return double(eMicroSeconds / 1000000) + double(eMicroSeconds % 1000000) * 0.000001;
}

CommunicationRequest::CommunicationRequest() : RegisterType("CommunicationRequest")
{
}

int CommunicationRequest::AddCommunicationComponentOption(string componentName, deque<CommunicationRequest*>* componentRequestQueue)
{
	LOG(LogLevel::Debug) << "int CommunicationRequest::AddCommunicationComponentOption() : component [" << componentName << "] get new request.";
	acceptedCommunicationComponentRequestQueues[componentName] = componentRequestQueue;

	return 0;
}

int CommunicationRequest::Cancel()
{
	exitRequested = true;
	return 0;
}

int CommunicationRequest::Fail(exception * e)
{
	//onFailed.Trigger();

	if (dynamic_cast<CommunicationRequestException*>(e)) {

	}

	return fail(e);
}

int CommunicationRequest::Success()
{
	return success();
}

int CommunicationRequest::SetCallbackScene(Scene * cScene)
{
	isCallbackByScene = true;
	callbackScene = cScene;
	return 0;
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

int CommunicationRequest::AddOnSuccess(ActionList<int>* actionsOnSuccess)
{
	onSuccess.Add(actionsOnSuccess);
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

int CommunicationRequest::writeTimePoint()
{
	// 計時點
	requestPointTime = system_clock::now();
	return 0;
}
