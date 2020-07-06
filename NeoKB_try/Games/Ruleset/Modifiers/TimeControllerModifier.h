#ifndef TIME_CONTROLLER_MODIFIER_H
#define TIME_CONTROLLER_MODIFIER_H


#include "../../../Framework/Timing/TimeController.h"
#include "Modifier.h"


using namespace Framework::Timing;



namespace Games {
namespace Rulesets{
namespace Modifiers{

	/// <summary>
	/// An interface for mods that make adjustments to the track.
	/// </summary>
	class TimeControllerModifier : virtual public Modifier {

	public:

		virtual int ApplyToTimeController(TimeController* timeController) = 0;

	};


}}}





#endif