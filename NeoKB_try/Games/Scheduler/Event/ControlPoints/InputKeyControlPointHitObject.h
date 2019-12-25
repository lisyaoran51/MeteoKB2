#ifndef INPUT_KEY_CONTROL_POINT_HIT_OBJECT_H
#define INPUT_KEY_CONTROL_POINT_HIT_OBJECT_H


#include "InputKeyControlPoint.h"
#include "../HitObject.h"
#include "../EventProcessor.h"


using namespace Games::Schedulers::Events;


namespace Games {
namespace Schedulers{
namespace Events{
namespace ControlPoints{

	class InputKeyControlPointHitObject : virtual public EventProcessor<Event>, public HitObject, public HasInputKey {

	public:

		InputKeyControlPointHitObject(HitWindow* hWindow);

		string GetTypeName();

		virtual double TryJudgement();

		virtual InputKeyControlPointHitObject* GetInputKeyControlPoint();

		virtual InputKey GetInputKey();

		virtual int GetInputValue();

	protected:


		virtual int CheckForJudgement(bool isUserTriggered, double timeOffset);


	};


}}}}






#endif