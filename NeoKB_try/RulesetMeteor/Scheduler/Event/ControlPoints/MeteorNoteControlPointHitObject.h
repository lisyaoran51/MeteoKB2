#ifndef METEOR_NOTE_CONTROL_POINT_HIT_OBJECT_H
#define METEOR_NOTE_CONTROL_POINT_HIT_OBJECT_H


#include "../../../../Games/Scheduler/Event/ControlPoints/NoteControlPointHitObject.h"



using namespace Games::Schedulers::Events::ControlPoints;



namespace Meteor {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	class MeteorNoteControlPointHitObject : public NoteControlPointHitObject {

	public:

		MeteorNoteControlPointHitObject();

		virtual Judgement* createJudgement(HitResult hResult);

		virtual Judgement* GetBestJudgement();

	protected:


		virtual HitWindow* createHitWindow();

	};

}}}}




#endif
