#ifndef METEOR_PATTERN_GENERATOR_H
#define METEOR_PATTERN_GENERATOR_H



#include <string>
#include <fstream>
#include <vector>
#include "../../../Games/Sheetmusic/Pattern/Pattern.h"
#include "../../../Games/Sheetmusic/Pattern/PatternGenerator.h"
#include "../../Config/MeteorConfigManager.h"
#include "../../../Games/Scheduler/Event/GameEvents/StartGameEvent.h"
#include "../../../Games/Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../../Games/Scheduler/Event/ControlPoints/InputKeyControlPoint.h"




using namespace std;
using namespace Util;
using namespace Games::Schedulers::Events;
using namespace Games::Sheetmusics;
using namespace Meteor::Config;
using namespace Games::Sheetmusics::Patterns;
using namespace Games::Schedulers::Events::GameEvents;
using namespace Games::Schedulers::Events::ControlPoints;



namespace Meteor {
namespace Sheetmusics {
namespace Patterns {

	/// <summary>
	/// to generate special pattern from original events
	///	</summary>
	class MeteorPatternGenerator: public PatternGenerator
	{

		int load();

		int load(MeteorConfigManager* mcf);

	public:

		MeteorPatternGenerator();

		virtual int Initialize(Sm<Event>* s);

		virtual Pattern* Generate(vector<Event*>* es, Event * e);

		/// <summary>
		/// 之後要移到post processor裡面，在sm converter裡面呼叫
		/// </summary>
		virtual int CreateOtherEvent(vector<Event*>* es);

		virtual int PostProcess();

	protected:

		float defaultVolume = 1.f;

		int extendWidth = 12;	// 應該要分左右邊不同的extend width

		MTO_FLOAT fallSpeed = 0;
		MTO_FLOAT fallBrightness = 0;
		int fallLength = 0;
		MTO_FLOAT explodeSpeed = 0;
		MTO_FLOAT explodeBrightness = 0;
		MTO_FLOAT glowLineSpeed = 0;
		MTO_FLOAT glowLineDuration = 0;
		MTO_FLOAT glowLineBrightness = 0;
		MTO_FLOAT targetLineBlinkSpeed = 0;
		int restartSection = 0;

		MTO_FLOAT pedalInterval = 0.2f;
		MTO_FLOAT defaultNextPedalTargetStartTime = 1.f;

		float defaultSectionInterval = 3.f;

		Pattern* generateNoteControlPoint(vector<Event*>* es, NoteControlPoint* note);
		Pattern* generateStartGameEvent(vector<Event*>* es, StartGameEvent* start);
		Pattern* generateInputKeyControlPoint(vector<Event*>* es, InputKeyControlPoint* inputKey);
		int generateRepeatPracticeEvents(vector<Event*>* es, vector<float>* sectionEndTime);

	};


}}}







#endif