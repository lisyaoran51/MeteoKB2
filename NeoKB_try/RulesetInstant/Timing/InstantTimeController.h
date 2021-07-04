#ifndef INSTANT_TIME_CONTROLLER_H
#define INSTANT_TIME_CONTROLLER_H

#include "../../Games/Timing/MeteoTimeController.h"
#include "../Input/InstantInputManager.h"


using namespace Instant::Input;
using namespace Games::Timing;


namespace Instant {
namespace Timing {


	class InstantTimeController : public MeteoTimeController<InstantAction> {

		int load();

	public:

		InstantTimeController();

	protected:

		virtual int onMessage(MeteoBluetoothMessage* message);

	};


}
}





#endif