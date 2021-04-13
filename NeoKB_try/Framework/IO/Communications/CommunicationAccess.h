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
		/// 把request丟給下面所有component，如果有component可以接，就會跑
		/// </summary>
		virtual int Queue(CommunicationRequest* communicationRequest);

		/// <summary>
		/// 這邊要用strategy處理wifi和ble同時處理的問題
		/// </summary>
		virtual int handleRequest(CommunicationRequest* communicationRequest);

		virtual int HandleState(InputState* inputEvent);

	protected:

		BleCommunicationInputHandler* bleCommunicationInputHandler = nullptr;
		
		int setHost(GameHost* gHost);

	};

}}}


#endif