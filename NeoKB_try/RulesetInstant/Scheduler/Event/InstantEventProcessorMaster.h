#ifndef INSTANT_EVENT_PROCESSOR_MASTER_H
#define INSTANT_EVENT_PROCESSOR_MASTER_H


#include "../../Input/InstantInputManager.h"
#include "../../../Games/Scheduler/Event/EventProcessorMaster.h"
#include "../../../Instruments/MeteoPiano.h"
#include <map>


using namespace Instant::Input;
using namespace Games::Schedulers::Events;
using namespace Instruments;
using namespace std;


namespace Instant {
namespace Schedulers{
namespace Events{

	class InstantEventProcessorMaster : public TEventProcessorMaster<InstantAction>{

	public:
		
		InstantEventProcessorMaster();

		int ChangePitchState(MeteoPianoPitchState pState);

		virtual int OnKeyDown(pair<InstantAction, int> action);

		virtual int OnKeyUp(InstantAction action);

		virtual int OnButtonDown(InstantAction action);

		virtual int OnButtonUp(InstantAction action);

		virtual int OnKnobTurn(pair<InstantAction, int> action);

		virtual int OnSlide(pair<InstantAction, int> action);

	protected:

		virtual int update();

		virtual int LoadOnComplete();

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		map<Pitch, InstantAction> pitchBindings;

		map<Pitch, InstantAction> loweredPitchBindings;

		map<Pitch, InstantAction> raisedPitchBindings;

		int loadAndMapPitches();

		bool matchPitch(HitObject* hObject, InstantAction meteorAction);

	};

}}}








#endif