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

	class CommunicationRequest {


	public:

		int AddCommunicationComponentOption(string componentName, deque<CommunicationRequest*>* componentRequestQueue);

		/// <summary>
		/// ���ɤ@��request�i�H�P�ɳQwifi�Bble�Bbt���h��communication component����A�o��request�n�������ثe�i��Ϊ�component�����ǡA�ۤv��w�n�έ���component
		/// �M��A����
		/// </summary>
		virtual int ChooseCommunicationComponentAndPerform() = 0;

		virtual int Perform(CommunicationComponent* cComponent) = 0;

		virtual int Fail(CommunicationRequestException& communicationRequestException);

		int SetCallbackScene(Scene* cScene);

		int AddOnSuccess(MtoObject * callableObject, function<int()> callback, string name = "OnRequestSuccess");
		int AddOnSuccess(ActionList<int>* actionsOnSuccess);

		int AddOnFailed(MtoObject * callableObject, function<int()> callback, string name = "OnRequestFailed");

		int AddOnCancelled(MtoObject * callableObject, function<int()> callback, string name = "OnRequestCancelled");


	protected:

		/// <summary>
		/// ���ɤ@��request�i�H�P�ɳQwifi�Bble�Bbt���h��communication component����A�o��request�n�������ثe�i��Ϊ�component�����ǡA�ۤv��w�n�έ���component
		/// �M��Arun
		/// </summary>
		map<string, deque<CommunicationRequest*>*> acceptedCommunicationComponentRequestQueues;

		system_clock::time_point requestCurrentTime;

		system_clock::time_point requestStartTime;

		/// <summary>
		/// �w�]5��Ntime out
		/// </summary>
		float timeout = 5;

		/// <summary>
		/// �w�]�̦h���s�T��
		/// </summary>
		int maxRetryCount = 3;

		int retryCount = 0;

		CommunicationComponent* communicationComponent = nullptr;

		Scene* callbackScene = nullptr;

		ActionList<int> onSuccess;

		ActionList<int> onFailed;

		ActionList<int> onCancelled;

		/// <summary>
		/// request�}�l�p��
		/// </summary>
		int requestTimeStart();

		/// <summary>
		/// ���o�ثe�������Ҵ��q���w�ӥήɶ��`�M�C(��stopwatch clock)
		/// </summary>
		long long getElapsedMicroseconds();

		double getElapsedSeconds();

		virtual int fail(CommunicationRequestException& communicationRequestException) = 0;


	};

	template<typename T>
	class TCommunicationRequest : virtual public CommunicationRequest {

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
