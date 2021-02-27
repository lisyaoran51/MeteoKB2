#ifndef RECORD_EVENT_PROCESSOR_MASTER_H
#define RECORD_EVENT_PROCESSOR_MASTER_H


#include "../../Input/RecordInputManager.h"
#include "../../../Games/Scheduler/Event/EventProcessorMaster.h"
#include "../../../Instruments/MeteoPiano.h"
#include <map>


using namespace Record::Input;
using namespace Games::Schedulers::Events;
using namespace Instruments;
using namespace std;


namespace Record {
namespace Schedulers{
namespace Events{

	class RecordEventProcessorMaster : public TEventProcessorMaster<RecordAction>{

	public:
		
		RecordEventProcessorMaster();

		int ChangePitchState(MeteoPianoPitchState pState);

		virtual int OnKeyDown(pair<RecordAction, int> action);

		virtual int OnKeyUp(RecordAction action);

		virtual int OnButtonDown(RecordAction action);

		virtual int OnButtonUp(RecordAction action);

		virtual int OnKnobTurn(pair<RecordAction, int> action);

		virtual int OnSlide(pair<RecordAction, int> action);

	protected:

		virtual int update();

		virtual int LoadOnComplete();

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		map<Pitch, RecordAction> pitchBindings;

		map<Pitch, RecordAction> loweredPitchBindings;

		map<Pitch, RecordAction> raisedPitchBindings;

		int loadAndMapPitches();

		bool matchPitch(HitObject* hObject, RecordAction meteorAction);

	};

}}}








#endif