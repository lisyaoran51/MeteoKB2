#ifndef RECORD_PATTERN_GENERATOR_H
#define RECORD_PATTERN_GENERATOR_H



#include <string>
#include <fstream>
#include <vector>
#include "../../../Games/Sheetmusic/Pattern/Pattern.h"
#include "../../../Games/Sheetmusic/Pattern/PatternGenerator.h"
#include "../../Config/RecordConfigManager.h"
#include "../../../Games/Scheduler/Event/GameEvents/StartGameEvent.h"
#include "../../../Games/Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../../Games/Scheduler/Event/ControlPoints/InputKeyControlPoint.h"




using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events;
using namespace Games::Sheetmusics;
using namespace Record::Config;
using namespace Games::Sheetmusics::Patterns;
using namespace Games::Schedulers::Events::GameEvents;
using namespace Games::Schedulers::Events::ControlPoints;



namespace Record {
namespace Sheetmusics {
namespace Patterns {

	/// <summary>
	/// to generate special pattern from original events
	///	</summary>
	class RecordPatternGenerator: public PatternGenerator
	{

		int load();

		int load(RecordConfigManager* mcf);

	public:

		RecordPatternGenerator();

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