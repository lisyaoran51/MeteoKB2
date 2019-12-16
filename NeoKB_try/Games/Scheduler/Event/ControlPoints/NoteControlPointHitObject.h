#ifndef NOTE_CONTROL_POINT_HIT_OBJECT_H
#define NOTE_CONTROL_POINT_HIT_OBJECT_H


#include "NoteControlPoint.h"
#include "../HitObject.h"
#include "../EventProcessor.h"


using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	class NoteControlPointHitObject : virtual public EventProcessor<Event>, public HitObject {

	public:

		NoteControlPointHitObject(HitWindow* hWindow);

		string GetTypeName();

		virtual double TryJudgement();

		virtual NoteControlPoint* GetNoteControlPoint();

	protected:


		virtual int CheckForJudgement(bool isUserTriggered, double offset);

	};

}}}}





#endif
