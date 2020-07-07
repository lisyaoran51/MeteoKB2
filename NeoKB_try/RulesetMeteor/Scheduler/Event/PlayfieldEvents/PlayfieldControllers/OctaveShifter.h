#ifndef OCTAVE_SHIFTER_H
#define OCTAVE_SHIFTER_H


#include "../../../../../Games/Scheduler/Event/PlayfieldEvents/PlayfieldControllers/PlayfieldController.h"
#include "../OctaveShiftEventProcessor.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapPitchShifter.h"



using namespace Games::Schedulers::Events::PlayfieldEvents::PlayfieldControllers;
using namespace Games::Schedulers::Events::PlayfieldEvents;
using namespace Games::Schedulers::Events::Effects::Algorithms;

namespace Meteor {
namespace Play{
	class MeteorPlayfield;
}}

using namespace Meteor::Play;

namespace Meteor {
namespace Schedulers{
namespace Events{
namespace PlayfieldEvents{
namespace PlayfieldControllers{

	/// <summary>
	/// 照理來說平移的速度應該是由map shifter來算，我們只給多久要移過去就好，但是map shifter懶得改寫法，所以平移的速度改由這邊來算
	/// </summary>
	class OctaveShifter : public PlayfieldController<OctaveShiftEvent> {


	public:

		OctaveShifter();

		virtual int LazyConstruct(Playfield* p);

	protected:

		MeteorPlayfield* meteorPlayfield = nullptr;

		MapPitchShifter* mapPitchShifter = nullptr;

		virtual int implementControlPlayfield(EventProcessor<Event>* eProcessor);

		virtual int implementUndoControlPlayfield(EventProcessor<Event>* eProcessor);



	};


}}}}}


#endif