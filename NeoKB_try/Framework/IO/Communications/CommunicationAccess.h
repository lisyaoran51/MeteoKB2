#ifndef COMMUNICATION_ACCESS_H
#define COMMUNICATION_ACCESS_H

#include "CommunicationCollectionManager.h"
#include "CommunicationRequest.h"
#include "../../Allocation/Hierachal/MtoObject.h"
#include "../../../Framework/Host/GameHost.h"
#include <deque>
#include "BleCommunicationInputHandler.h"



using namespace Framework::Allocation::Hierachal;
using namespace Framework::Host;
using namespace std;


namespace Framework {
namespace IO{
namespace Communications{

	class CommunicationAccess : public MtoObject, public CommunicationCollectionManager<CommunicationComponent> {

	public:

		CommunicationAccess(GameHost* gHost);

		/// <summary>
		/// ��request�ᵹ�U���Ҧ�component�A�p�G��component�i�H���A�N�|�]
		/// </summary>
		virtual int Queue(CommunicationRequest* communicationRequest);

		/// <summary>
		/// �o��n��strategy�B�zwifi�Mble�P�ɳB�z�����D
		/// </summary>
		virtual int handleRequest(CommunicationRequest* communicationRequest);

		virtual int HandleState(InputState* inputEvent);

	protected:

		BleCommunicationInputHandler* bleCommunicationInputHandler = nullptr;
		
		int setHost(GameHost* gHost);

	};

}}}


#endif