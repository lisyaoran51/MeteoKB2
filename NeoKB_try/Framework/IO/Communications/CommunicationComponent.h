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
		/// 執行scheduler的代辦工作
		/// </summary>
		virtual int Update();

	protected:

		StopwatchClock* sourceClock = nullptr;

		FrameBasedClock* framedClock = nullptr;

		Scheduler* scheduler = nullptr;

		GameHost* host = nullptr;

		CommunicationState communicationState = CommunicationState::None;

		/// <summary>
		/// 獨力跑的thread，再建構子new，等到被連線就開始跑回圈
		/// TODO: 把這個thread註冊到host裡
		/// </summary>
		GameThread* communicationThread = nullptr;

		int failureCount = 0;

		virtual int run();

		/// <summary>
		/// 這邊要用strategy處理wifi和ble同時處理的問題
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
			// TODO: 把所有request清掉
			return 0;
		}

	protected:

		deque<CommunicationRequest*> communicationRequests;

	};

}}}





#endif