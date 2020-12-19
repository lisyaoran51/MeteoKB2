#ifndef COMMUNICATION_REQUEST_H
#define COMMUNICATION_REQUEST_H


#include <chrono>



using namespace std::chrono;



namespace Framework {
namespace IO{
namespace Communications{

	class CommunicationRequest {


		/// <summary>
		/// ���o�ثe�������Ҵ��q���w�ӥήɶ��`�M�C(��stopwatch clock)
		/// </summary>
		long long getElapsedMicroseconds();

		double getElapsedSeconds();

	public:




	protected:

		system_clock::time_point systemCurrentTime;

		system_clock::time_point systemStartTime;

		/// <summary>
		/// 0.01��Nfail
		/// </summary>
		float timeout = 0.01;

		ActionList<int> onSuccess;

		ActionList<int> onFailed;

		ActionList<int> onCancelled;


	};

	template<typename T>
	class TCommunicationRequest : public CommunicationRequest {

	public:

		TCommunicationRequest() {
			onSuccess.Add(this, [=]() {
				onTSuccess
				return 0;
			}
				)
		}

		T 

	protected:

		ActionList<int, T> onTSuccess;

	};



}}}





#endif
