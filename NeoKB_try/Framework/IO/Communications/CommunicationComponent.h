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
		/// �W�O�]��thread�A�A�غc�lnew�A����Q�s�u�N�}�l�]�^��
		/// TODO: ��o��thread���U��host��
		/// </summary>
		GameThread* communicationThread = nullptr;



		int run();

		/// <summary>
		/// �o��n��strategy�B�zwifi�Mble�P�ɳB�z�����D
		/// </summary>
		virtual int handleRequest(CommunicationRequest* communicationRequest) = 0;

	};


}}}





#endif