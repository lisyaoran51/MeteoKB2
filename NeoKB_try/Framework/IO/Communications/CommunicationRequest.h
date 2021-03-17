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

		// �ѦҦ� https://stackoverflow.com/questions/8152720/correct-way-to-inherit-from-stdexception

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
		/// ���ɤ@��request�i�H�P�ɳQwifi�Bble�Bbt���h��communication component����A�o��request�n�������ثe�i��Ϊ�component�����ǡA�ۤv��w�n�έ���component
		/// �M��A����
		/// </summary>
		virtual int ChooseCommunicationComponentToPerform() = 0;

		virtual int Perform(CommunicationComponent* cComponent) = 0;

		/// <summary>
		/// ��exit request��true�A�i�H��request���}perform thread
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
		/// ���ɤ@��request�i�H�P�ɳQwifi�Bble�Bbt���h��communication component����A�o��request�n�������ثe�i��Ϊ�component�����ǡA�ۤv��w�n�έ���component
		/// �M��Arun
		/// </summary>
		map<string, deque<CommunicationRequest*>*> acceptedCommunicationComponentRequestQueues;

		system_clock::time_point requestCurrentTime;

		system_clock::time_point requestStartTime;

		/// <summary>
		/// �Y�Ӭq�����ɶ��A���ɭԥu�n��Y�Ӭq���g�L�h�[�A�N�H�o�Өӭp��
		/// </summary>
		system_clock::time_point requestPointTime;

		/// <summary>
		/// �w�]5��Ntime out
		/// </summary>
		float timeout = 5;

		/// <summary>
		/// �w�]�̦h���s�T��
		/// </summary>
		int maxRetryCount = 3;

		int retryCount = 0;

		/// <summary>
		/// �bthread�~���I�s�A�i�H�����bperform��thread����
		/// </summary>
		bool exitRequested = false;

		/// <summary>
		/// �o�ӨS���ΤF
		/// </summary>
		CommunicationComponent* communicationComponent = nullptr;

		bool isCallbackByScene = true;

		Scene* callbackScene = nullptr;

		ActionList<int> onSuccess;

		ActionList<int> onFailed;

		ActionList<int> onCancelled;

		/// <summary>
		/// request�}�l�p��
		/// </summary>
		int requestTimeStart();

		/// <summary>
		/// request���J�p���I
		/// </summary>
		int writeTimePoint();

		/// <summary>
		/// ���o�ثe�������Ҵ��q���w�ӥήɶ��`�M�C(��stopwatch clock)
		/// </summary>
		long long getElapsedMicroseconds();

		double getElapsedSeconds();

		/// <summary>
		/// ���o�ثe�������q���J�p���I��{�b�үӪ��ɶ��`�M(�����϶�)
		/// </summary>
		long long getSectionElapsedMicroseconds();

		/// <summary>
		/// ���o�ثe�������q���J�p���I��{�b�үӪ��ɶ��`�M(�����϶�)
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
