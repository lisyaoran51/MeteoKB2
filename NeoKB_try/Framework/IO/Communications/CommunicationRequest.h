#ifndef COMMUNICATION_REQUEST_H
#define COMMUNICATION_REQUEST_H


#include <chrono>
#include <deque>
#include <exception>
#include "../../Allocation/Hierachal/MtoObject.h"
#include <functional>
#include "../../../Util/DataStructure/ActionList.h"
#include <map>



using namespace std::chrono;
using namespace std;
using namespace Framework::Allocation::Hierachal;
using namespace Util::DataStructure;


namespace Framework {
namespace IO{
namespace Communications{

	class CommunicationComponent;

	class CommunicationRequest;

	class CommunicationRequestException : public exception {

	};

	class CommunicationRequest {


		/// <summary>
		/// ���o�ثe�������Ҵ��q���w�ӥήɶ��`�M�C(��stopwatch clock)
		/// </summary>
		long long getElapsedMicroseconds();

		double getElapsedSeconds();

	public:

		int AddCommunicationComponentOption(string componentName, deque<CommunicationRequest*>* componentRequestQueue);

		/// <summary>
		/// ���ɤ@��request�i�H�P�ɳQwifi�Bble�Bbt���h��communication component����A�o��request�n�������ثe�i��Ϊ�component�����ǡA�ۤv��w�n�έ���component
		/// �M��A����
		/// </summary>
		virtual int ChooseCommunicationComponentAndPerform() = 0;

		virtual int Perform(CommunicationComponent* cComponent) = 0;

		virtual int Fail(CommunicationRequestException& communicationRequestException);

		int AddOnSuccess(MtoObject * callableObject, function<int()> callback, string name);

		int AddOnFailed(MtoObject * callableObject, function<int()> callback, string name);

		int AddOnCancelled(MtoObject * callableObject, function<int()> callback, string name);


	protected:

		/// <summary>
		/// ���ɤ@��request�i�H�P�ɳQwifi�Bble�Bbt���h��communication component����A�o��request�n�������ثe�i��Ϊ�component�����ǡA�ۤv��w�n�έ���component
		/// �M��Arun
		/// </summary>
		map<string, deque<CommunicationRequest*>*> acceptedCommunicationComponentRequestQueues;

		system_clock::time_point systemCurrentTime;

		system_clock::time_point systemStartTime;

		/// <summary>
		/// �w�]0.01��Nfail
		/// </summary>
		float timeout = 0.01;

		/// <summary>
		/// �w�]�̦h���s�T��
		/// </summary>
		int tryCountLeft = 3;

		int retryCount = 0;

		CommunicationComponent* communicationComponent = nullptr;

		ActionList<int> onSuccess;

		ActionList<int> onFailed;

		ActionList<int> onCancelled;

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
