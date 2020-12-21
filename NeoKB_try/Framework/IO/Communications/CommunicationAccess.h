#ifndef COMMUNICATION_ACCESS_H
#define COMMUNICATION_ACCESS_H

#include "CommunicationCollectionManager.h"
#include "CommunicationRequest.h"
#include "../../Allocation/Hierachal/Container.h"
#include "../../../Framework/Host/GameHost.h"
#include <deque>



using namespace Framework::Allocation::Hierachal;
using namespace Framework::Host;
using namespace std;


namespace Framework {
namespace IO{
namespace Communications{

	class CommunicationAccess : public Container, public CommunicationCollectionManager<CommunicationComponent> {

	public:

		CommunicationAccess(GameHost* gHost);

		/// <summary>
		/// 把request丟給下面所有component，如果有component可以接，就會跑
		/// </summary>
		virtual int Queue(CommunicationRequest* communicationRequest);

	protected:

		

	};

}}}


#endif