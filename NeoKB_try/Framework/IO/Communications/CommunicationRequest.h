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

		// 參考自 https://stackoverflow.com/questions/8152720/correct-way-to-inherit-from-stdexception

		CommunicationRequestException(){}

		/** Constructor (C strings).
		*  @param message C-style string error message.
		*                 The string contents are copied upon construction.
		*                 Hence, responsibility for deleting the char* lies
		*                 with the caller.
		*/
		explicit CommunicationRequestException(const char* message)
			: msg_(message) {}

		/** Constructor (C++ STL strings).
		 *  @param message The error message.
		 */
		explicit CommunicationRequestException(const std::string& message)
			: msg_(message) {}

		/** Destructor.
		 * Virtual to allow for subclassing.
		 */
		virtual ~CommunicationRequestException() noexcept {}

		/** Returns a pointer to the (constant) error description.
		 *  @return A pointer to a const char*. The underlying memory
		 *          is in posession of the Exception object. Callers must
		 *          not attempt to free the memory.
		 */
		virtual const char* what() const noexcept {
			return msg_.c_str();
		}

	protected:
		/** Error message.
		 */
		std::string msg_;

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
		system_clock::time_point requestPointTime;

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
		bool exitRequested = false;

		/// <summary>
		/// 這個沒有用了
		/// </summary>
		CommunicationComponent* communicationComponent = nullptr;

		bool isCallbackByScene = true;

		Scene* callbackScene = nullptr;

		ActionList<int> onSuccess;

		ActionList<int> onFailed;

		ActionList<int> onCancelled;

		/// <summary>
		/// request開始計時
		/// </summary>
		int requestTimeStart();

		/// <summary>
		/// request插入計時點
		/// </summary>
		int writeTimePoint();

		/// <summary>
		/// 取得目前執行個體所測量的已耗用時間總和。(看stopwatch clock)
		/// </summary>
		long long getElapsedMicroseconds();

		double getElapsedSeconds();

		/// <summary>
		/// 取得目前執行個體從插入計時點到現在所耗的時間總和(部分區間)
		/// </summary>
		long long getSectionElapsedMicroseconds();

		/// <summary>
		/// 取得目前執行個體從插入計時點到現在所耗的時間總和(部分區間)
		/// </summary>
		double getSectionElapsedSeconds();

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
