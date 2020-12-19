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

	protected:


	};

}}}


#endif