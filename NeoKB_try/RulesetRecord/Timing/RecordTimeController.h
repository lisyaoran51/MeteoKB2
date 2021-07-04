#ifndef RECORD_TIME_CONTROLLER_H
#define RECORD_TIME_CONTROLLER_H

#include "../../Games/Timing/MeteoTimeController.h"
#include "../Input/RecordInputManager.h"


using namespace Record::Input;
using namespace Games::Timing;


namespace Record {
namespace Timing {


	class RecordTimeController : public MeteoTimeController<RecordAction> {

		int load();

	public:

		RecordTimeController();

	protected:

		virtual int onMessage(MeteoBluetoothMessage* message);

	};


}
}





#endif