#ifndef METEOR_INPUT_KEY_CONTROL_POINT_HIT_OBJECT_H
#define METEOR_INPUT_KEY_CONTROL_POINT_HIT_OBJECT_H


#include "../../../../Games/Scheduler/Event/ControlPoints/InputKeyControlPointHitObject.h"



using namespace Games::Schedulers::Events::ControlPoints;



namespace Meteor {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	class MeteorInputKeyControlPointHitObject : public InputKeyControlPointHitObject {

	public:

		MeteorInputKeyControlPointHitObject();

		virtual Judgement* createJudgement(HitResult hResult);

		virtual Judgement* GetBestJudgement();
	};

}}}}




#endif
