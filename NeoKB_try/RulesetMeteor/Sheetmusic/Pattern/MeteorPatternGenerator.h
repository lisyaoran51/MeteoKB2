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
#include "../../../Games/Scheduler/Event/ControlPoints/SectionStartControlPoint.h"




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

		float defaultSectionInterval = 5.f;

		/// <summary>
		/// 切換目前的音域，用來改變effect出現的位置。重點是所有事件要照順序排列，不然會轉換音域失敗。而且如果轉換音域和音符在同一瞬間，也會失敗
		/// </summary>
		MeteorEffectShiftType meteorEffectShiftType = MeteorEffectShiftType::None;

		Pattern* generateNoteControlPoint(vector<Event*>* es, NoteControlPoint* note);
		Pattern* generateStartGameEvent(vector<Event*>* es, StartGameEvent* start);
		Pattern* generateInputKeyControlPoint(vector<Event*>* es, InputKeyControlPoint* inputKey);

		/// <summary>
		/// 建立一整首歌的所有反覆練習事件
		/// </summary>
		int generateRepeatPracticeEvents(vector<Event*>* es, vector<float>* sectionEndTime);

		/// <summary>
		/// 建立單個反覆練習事件
		/// </summary>
		Pattern* generateRepeatPracticeEvent(vector<Event*>* es, SectionStartControlPoint* sectionStart);

	};


}}}







#endif