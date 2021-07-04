#ifndef INSTANT_OCTAVE_SHIFTER_H
#define INSTANT_OCTAVE_SHIFTER_H


#include "../../../../../Games/Scheduler/Event/PlayfieldEvents/PlayfieldControllers/PlayfieldController.h"
#include "../InstantOctaveShiftEventProcessor.h"
#include "../../../../../Games/Scheduler/Event/Effect/Algorithm/MapPitchShifter.h"



using namespace Games::Schedulers::Events::PlayfieldEvents::PlayfieldControllers;
using namespace Games::Schedulers::Events::PlayfieldEvents;
using namespace Games::Schedulers::Events::Effects::Algorithms;

namespace Instant {
namespace Scenes {
namespace Play {
	class InstantPlayfield;
}}}

using namespace Instant::Scenes::Play;

namespace Instant {
namespace Schedulers{
namespace Events{
namespace PlayfieldEvents{
namespace PlayfieldControllers{

	/// <summary>
	/// �Ӳz�ӻ��������t�����ӬO��map shifter�Ӻ�A�ڭ̥u���h�[�n���L�h�N�n�A���Omap shifter�i�o��g�k�A�ҥH�������t�ק�ѳo��Ӻ�
	/// </summary>
	class InstantOctaveShifter : public PlayfieldController<InstantOctaveShiftEvent> {


	public:

		InstantOctaveShifter();

		virtual int LazyConstruct(Playfield* p);

	protected:

		InstantPlayfield* instantPlayfield = nullptr;

		virtual int implementControlPlayfield(EventProcessor<Event>* eProcessor);

		virtual int implementUndoControlPlayfield(EventProcessor<Event>* eProcessor);

	};


}}}}}


#endif