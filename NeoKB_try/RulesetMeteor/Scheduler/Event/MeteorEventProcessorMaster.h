#ifndef METEOR_EVENT_PROCESSOR_MASTER_H
#define METEOR_EVENT_PROCESSOR_MASTER_H


#include "../../Input/MeteorInputManager.h"
#include "../../../Games/Scheduler/Event/EventProcessorMaster.h"
#include "../../../Instruments/MeteoPiano.h"
#include <map>


using namespace Meteor::Input;
using namespace Games::Schedulers::Events;
using namespace Instruments;
using namespace std;


namespace Meteor {
namespace Schedulers{
namespace Events{

	class MeteorEventProcessorMaster : public TEventProcessorMaster<MeteorAction>{

		int load();

	public:
		
		MeteorEventProcessorMaster();

		/// <summary>
		/// 這邊是用map的value反推回去，有很大的效率問題，不過因為不常使用，所以不做雙向map
		/// </summary>
		virtual Pitch GetPitchFromAction(MeteorAction action);

		virtual int OnKeyDown(pair<MeteorAction, int> action);

		virtual int OnKeyUp(MeteorAction action);

		virtual int OnButtonDown(MeteorAction action);

		virtual int OnButtonUp(MeteorAction action);

		virtual int OnKnobTurn(pair<MeteorAction, int> action);

		virtual int OnSlide(pair<MeteorAction, int> action);

	protected:

		virtual int update();

		virtual int LoadOnComplete();

		map<Pitch, MeteorAction> pitchBindings;

		map<Pitch, MeteorAction> loweredPitchBindings;

		map<Pitch, MeteorAction> raisedPitchBindings;

		int loadAndMapPitches();

		bool matchPitch(HitObject* hObject, MeteorAction meteorAction);

		bool filterHiddenNoteEffects(EventProcessor<Event>* eventProcessor);

	};

}}}








#endif