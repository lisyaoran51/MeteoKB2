#ifndef COMMUNICATION_COMPONENT_H
#define COMMUNICATION_COMPONENT_H



#include "../../../Framework/Host/GameHost.h"
#include "../../../Framework/Timing/StopwatchClock.h"
#include <deque>
#include "../../../Framework/Allocation/Hierachal/MtoObject.h"
#include "CommunicationRequest.h"



using namespace Framework::Host;
using namespace std;
using namespace Framework::Allocation::Hierachal;
using namespace Framework::Timing;


namespace Framework {
namespace IO{
namespace Communications{

	enum class CommunicationState {
		None,
		Offline,
		Listening,
		Connecting,
		Connected,
		Failed,
		Disconnected
	};


	class CommunicationComponent : virtual public RegisterType {

	public:

		CommunicationComponent(GameHost* gHost);

		/// <summary>
		/// �إ�thread�åB�}�l�]
		/// </summary>
		virtual int Start();

		CommunicationState GetCommunicationState();

		GameThread* GetCommunicationThread();

		int SetSourceClock(StopwatchClock* sClock);

		StopwatchClock* GetSourceClock();

		int InitializeFramedClockAndScheduler();

		FrameBasedClock* GetFramedClock();

		Scheduler* GetScheduler();

		virtual int Queue(CommunicationRequest*) = 0;

		virtual int Flush() = 0;

		/// <summary>
		/// ����scheduler���N��u�@
		/// </summary>
		virtual int Update();

	protected:

		thread* runThread = nullptr;

		StopwatchClock* sourceClock = nullptr;

		FrameBasedClock* framedClock = nullptr;

		Scheduler* scheduler = nullptr;

		GameHost* host = nullptr;

		CommunicationState communicationState = CommunicationState::None;

		/// <summary>
		/// �W�O�]��thread�A�A�غc�lnew�A����Q�s�u�N�}�l�]�^��
		/// TODO: ��o��thread���U��host��
		/// </summary>
		GameThread* communicationThread = nullptr;

		/// <summary>
		/// ���ܦ�true���ɭԡArun�N�|���X�A��thread����
		/// </summary>
		bool threadExitRequest = false;

		int failureCount = 0;

		virtual int run();

		/// <summary>
		/// �o��n��strategy�B�zwifi�Mble�P�ɳB�z�����D
		/// </summary>
		virtual int handleRequest(CommunicationRequest* communicationRequest) = 0;

	};

	template<typename T>
	class TCommunicationComponent : public CommunicationComponent {

	public:

		TCommunicationComponent(GameHost* gHost) : CommunicationComponent(gHost), RegisterType("TCommunicationComponent"){
		}

		virtual int Queue(CommunicationRequest* communicationRequest) {
			if (dynamic_cast<T*>(communicationRequest) &&
				GetCommunicationState() == CommunicationState::Connected) {

				communicationRequest->AddCommunicationComponentOption(GetTypeName(), &communicationRequests);
				
			}
			return 0;
		}

		virtual int Flush() {

			deque<CommunicationRequest*> oldQueue;
			copy(communicationRequests.begin(), communicationRequests.end(), inserter(oldQueue, oldQueue.begin()));
			communicationRequests.clear();

			while (oldQueue.size() > 0) {
				CommunicationRequest* request = oldQueue.back();
				request->Fail(CommunicationRequestException("Disconnected from communication target."));
				oldQueue.pop_back();

				delete request;
				request = nullptr;
			}

			return 0;
		}

	protected:

		deque<CommunicationRequest*> communicationRequests;

	};

}}}





#endif