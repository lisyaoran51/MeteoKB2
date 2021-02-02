#ifndef MUSIC_GAME_MODIFIER_H
#define MUSIC_GAME_MODIFIER_H




#include "../../../Games/Ruleset/Modifiers/Modifier.h"
#include "../../../Games/Ruleset/Modifiers/EventProcessorFilterModifier.h"
#include "../../../Games/Scheduler/Event/Event.h"


using namespace Games::Rulesets::Modifiers;
using namespace Games::Schedulers::Events;

namespace Meteor {
namespace Rulesets{
namespace Modifiers{

	class MusicGameModifier : public EventProcessorFilterModifier {

	public:

		MusicGameModifier();

		/// <summary>
		/// 要拿掉erupt和rewind兩種event
		/// </summary>
		int ApplyToEventProcessorFilter(EventProcessorFilter* eventProcessorFilter);

	protected:

		virtual bool filterRepeatPracticeEventProcessors(EventProcessor<Event>* eventProcessor);

		virtual bool filterEruptEffects(EventProcessor<Event>* eventProcessor);

	};



}}}



#endif