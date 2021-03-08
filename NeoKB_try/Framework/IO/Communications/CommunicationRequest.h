#ifndef COMMUNICATION_REQUEST_H
#define COMMUNICATION_REQUEST_H


#include <chrono>
#include <deque>
#include <exception>
#include "../../Allocation/Hierachal/MtoObject.h"
#include <functional>
#include "../../../Util/DataStructure/ActionList.h"
#include <map>
#include "../../Scenes/Scene.h"



using namespace std::chrono;
using namespace std;
using namespace Framework::Allocation::Hierachal;
using namespace Util::DataStructure;
using namespace Framework::Scenes;


namespace Framework {
namespace IO{
namespace Communications{

	class CommunicationComponent;

	class CommunicationRequestException : public exception {

	};

	class CommunicationRequest : public MtoObject {


	public:

		CommunicationRequest();

		int AddCommunicationComponentOption(string componentName, deque<CommunicationRequest*>* componentRequestQueue);

		/// <summary>
		/// 有時一個request可以同時被wifi、ble、bt等多種communication component執行，這時request要先接收目前可選用的component有哪些，自己選定要用哪種component
		/// 然後再執行
		/// </summary>
		virtual int ChooseCommunicationComponentToPerform() = 0;

		virtual int Perform(CommunicationComponent* cComponent) = 0;

		/// <summary>
		/// 把exit request變true，可以讓request離開perform thread
		/// </summary>
		int Cancel();

		virtual int Fail(exception& e);

		int SetCallbackScene(Scene* cScene);

		int AddOnSuccess(MtoObject * callableObject, function<int()> callback, string name = "OnRequestSuccess");
		int AddOnSuccess(ActionList<int>* actionsOnSuccess);

		int AddOnFailed(MtoObject * callableObject, function<int()> callback, string name = "OnRequestFailed");

		int AddOnCancelled(MtoObject * callableObject, function<int()> callback, string name = "OnRequestCancelled");


	protected:

		/// <summary>
		/// 有時一個request可以同時被wifi、ble、bt等多種communication component執行，這時request要先接收目前可選用的component有哪些，自己選定要用哪種component
		/// 然後再run
		/// </summary>
		map<string, deque<CommunicationRequest*>*> acceptedCommunicationComponentRequestQueues;

		system_clock::time_point requestCurrentTime;

		system_clock::time_point requestStartTime;

		/// <summary>
		/// 某個段落的時間，有時候只要抓某個段落經過多久，就以這個來計時
		/// </summary>
		system_clock::time_point requestpointTime;

		/// <summary>
		/// 預設5秒就time out
		/// </summary>
		float timeout = 5;

		/// <summary>
		/// 預設最多重連三次
		/// </summary>
		int maxRetryCount = 3;

		int retryCount = 0;

		/// <summary>
		/// 在thread外面呼叫，可以讓正在perform的thread結束
		/// </summary>
		bool exitRequest = false;

		/// <summary>
		/// 這個沒有用了
		/// </summary>
		CommunicationComponent* communicationComponent = nullptr;

		Scene* callbackScene = nullptr;

		ActionList<int> onSuccess;

		ActionList<int> onFailed;

		ActionList<int> onCancelled;

		/// <summary>
		/// request開始計時
		/// </summary>
		int requestTimeStart();

		/// <summary>
		/// 取得目前執行個體所測量的已耗用時間總和。(看stopwatch clock)
		/// </summary>
		long long getElapsedMicroseconds();

		double getElapsedSeconds();

		virtual int fail(exception& e) = 0;


	};

	template<typename T>
	class TCommunicationRequest : virtual public CommunicationRequest {

	public:

		TCommunicationRequest() : RegisterType("TCommunicationRequest") {
			onSuccess.Add(this, [=](T rObject) {
				return onTSuccess(rObject);
				
			}(responseObject));
		}


		int AddOnTSuccess(MtoObject * callableObject, function<int(T)> callback, string name) {
			return onTSuccess.Add(callableObject, callback, name);
		}

	protected:

		T responseObject;

		ActionList<int, T> onTSuccess;

	};

	template<typename T>
	class TTransferCommunicationRequest : public TCommunicationRequest<T> {

	public:

		TTransferCommunicationRequest() : RegisterType("TTransferCommunicationRequest"){}


		int AddOnProgress(MtoObject * callableObject, function<int(float)> callback, string name) {
			return onProgress.Add(callableObject, callback, name);
		}

	protected:

		ActionList<int, float> onProgress;

	};

}}}





#endif
