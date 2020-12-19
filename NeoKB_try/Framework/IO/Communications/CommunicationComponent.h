#ifndef COMMUNICATION_COMPONENT_H
#define COMMUNICATION_COMPONENT_H



#include "../../../Framework/Host/GameHost.h"



using namespace Framework::Host;


namespace Framework {
namespace IO{
namespace Communications{

	enum class CommunicationState {
		None,
		Listening,
		Connecting,
		Connected,
		Failed,
		Disconnected
	};


	class CommunicationComponent {

	public:

		CommunicationComponent(GameHost* gHost);

		CommunicationState GetCommunicationState();

		int Queue(CommunicationRequest*);

		GameThread* GetCommunicationThread();



	protected:

		CommunicationState communicationState = CommunicationState::None;

		deque<CommunicationRequest*> communicationRequests;


		/// <summary>
		/// 獨力跑的thread，再建構子new，等到被連線就開始跑回圈
		/// TODO: 把這個thread註冊到host裡
		/// </summary>
		GameThread* communicationThread = nullptr;



		int run();

		/// <summary>
		/// 這邊要用strategy處理wifi和ble同時處理的問題
		/// </summary>
		virtual int handleRequest(CommunicationRequest* communicationRequest) = 0;

	};


}}}





#endif