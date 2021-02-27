#ifndef RECORD_TIME_CONTROLLER_H
#define RECORD_TIME_CONTROLLER_H

#include "../../Framework/Timing/TimeController.h"
#include "../Input/RecordInputManager.h"


using namespace Record::Input;
using namespace Framework::Timing;


namespace Record {
namespace Timing {


	class RecordTimeController : public TimeController {

		int load();

	public:

		RecordTimeController();

	protected:

	};


}
}





#endif