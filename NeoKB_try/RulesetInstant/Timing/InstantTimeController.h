#ifndef INSTANT_TIME_CONTROLLER_H
#define INSTANT_TIME_CONTROLLER_H

#include "../../Framework/Timing/TimeController.h"
#include "../Input/InstantInputManager.h"


using namespace Instant::Input;
using namespace Framework::Timing;


namespace Instant {
namespace Timing {


	class InstantTimeController : public TimeController {

		int load();

	public:

		InstantTimeController();

	protected:

	};


}
}





#endif