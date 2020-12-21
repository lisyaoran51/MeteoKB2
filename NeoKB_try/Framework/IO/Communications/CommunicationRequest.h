#ifndef COMMUNICATION_REQUEST_H
#define COMMUNICATION_REQUEST_H


#include <chrono>
#include <deque>



using namespace std::chrono;



namespace Framework {
namespace IO{
namespace Communications{

	class CommunicationRequest;

	class CommunicationRequest {


		/// <summary>
		/// 取得目前執行個體所測量的已耗用時間總和。(看stopwatch clock)
		/// </summary>
		long long getElapsedMicroseconds();

		double getElapsedSeconds();

	public:

		int AddCommunicationComponentOption(string componentName, deque<CommunicationRequest*>* componentRequestQueue);

		/// <summary>
		/// 有時一個request可以同時被wifi、ble、bt等多種communication component執行，這時request要先接收目前可選用的component有哪些，自己選定要用哪種component
		/// 然後再執行
		/// </summary>
		virtual int ChooseCommunicationComponentAndPerform() = 0;

		int AddOnSuccess(MtoObject * callableObject, function<int()> callback, string name);

		int AddOnFailed(MtoObject * callableObject, function<int()> callback, string name);

		int AddOnCancelled(MtoObject * callableObject, function<int()> callback, string name);


	protected:

		/// <summary>
		/// 有時一個request可以同時被wifi、ble、bt等多種communication component執行，這時request要先接收目前可選用的component有哪些，自己選定要用哪種component
		/// 然後再run
		/// </summary>
		map<string, deque<CommunicationRequest*>*> acceptedCommunicationComponentRequestQueues;

		system_clock::time_point systemCurrentTime;

		system_clock::time_point systemStartTime;

		/// <summary>
		/// 預設0.01秒就fail
		/// </summary>
		float timeout = 0.01;

		/// <summary>
		/// 預設最多重連三次
		/// </summary>
		int tryCountLeft = 3;

		ActionList<int> onSuccess;

		ActionList<int> onFailed;

		ActionList<int> onCancelled;


	};

	template<typename T>
	class TCommunicationRequest : public CommunicationRequest {

	public:

		TCommunicationRequest() {
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
	class TDownloadCommunicationRequest : public TCommunicationRequest<T> {

	public:


		int AddOnProgress(MtoObject * callableObject, function<int(float)> callback, string name) {
			return onProgress.Add(callableObject, callback, name);
		}

	protected:

		ActionList<int, float> onProgress;

	};

}}}





#endif
