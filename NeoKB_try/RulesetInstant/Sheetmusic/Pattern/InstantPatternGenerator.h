#ifndef INSTANT_PATTERN_GENERATOR_H
#define INSTANT_PATTERN_GENERATOR_H



#include <string>
#include <fstream>
#include <vector>
#include "../../../Games/Sheetmusic/Pattern/Pattern.h"
#include "../../../Games/Sheetmusic/Pattern/PatternGenerator.h"
#include "../../Config/InstantConfigManager.h"
#include "../../../Games/Scheduler/Event/GameEvents/StartGameEvent.h"
#include "../../../Games/Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../../Games/Scheduler/Event/ControlPoints/InputKeyControlPoint.h"




using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events;
using namespace Games::Sheetmusics;
using namespace Instant::Config;
using namespace Games::Sheetmusics::Patterns;
using namespace Games::Schedulers::Events::GameEvents;
using namespace Games::Schedulers::Events::ControlPoints;



namespace Instant {
namespace Sheetmusics {
namespace Patterns {

	/// <summary>
	/// to generate special pattern from original events
	///	</summary>
	class InstantPatternGenerator: public PatternGenerator
	{

		int load();

		int load(InstantConfigManager* mcf);

	public:

		InstantPatternGenerator();

		virtual int Initialize(Sm<Event>* s);

		virtual Pattern* Generate(vector<Event*>* es, Event * e);

		/// <summary>
		/// 之後要移到post processor裡面，在sm converter裡面呼叫
		/// </summary>
		virtual int CreateOtherEvent(vector<Event*>* es);

		virtual int PostProcess();

	protected:


	};


}}}







#endif