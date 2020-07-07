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
	/// �Ӳz�ӻ��������t�����ӬO��map shifter�Ӻ�A�ڭ̥u���h�[�n���L�h�N�n�A���Omap shifter�i�o��g�k�A�ҥH�������t�ק�ѳo��Ӻ�
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