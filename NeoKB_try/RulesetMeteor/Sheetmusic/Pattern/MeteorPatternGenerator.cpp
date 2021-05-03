#include "MeteorPatternGenerator.h"

#include "../../../Util/MtoType.h"
#include "../../../Games/Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../../Games/Scheduler/Event/ControlPoints/InputKeyControlPoint.h"
#include "../../Scheduler/Event/Effect/FallEffect.h"
#include "../../Scheduler/Event/Effect/GlowLineEffect.h"
#include "../../Scheduler/Event/Effect/TargetLineEffect.h"
#include "../../../Games/Scheduler/Event/GameEvents/StartGameEvent.h"
#include "../../../Games/Scheduler/Event/SystemEvents/SystemEvent.h"
#include "../../../Games/Scheduler/Event/SystemEvents/StopSystemEvent.h"
#include "../../../Instruments/Pitch.h"
#include "../../Scheduler/Event/IoEvents/SustainPedalIoEvent.h"
#include "../../Scheduler/Event/InstrumentEvents/PianoEvent.h"
#include "../../Scheduler/Event/InstrumentEvents/PianoSoundEvent.h"
#include "../../Scheduler/Event/PlayfieldEvents/OctaveShiftEvent.h"
#include "../../Scheduler/Event/TimeEvents/RepeatPracticeEvent.h"
#include "../../Scheduler/Event/Effect/EruptEffect.h"
#include <math.h>
//#include "../../Scheduler/Event/RecorderEvents/MeteorButtonRecorderEvent.h"



using namespace Meteor::Sheetmusics::Patterns;
using namespace Meteor::Config;
using namespace Games::Schedulers::Events;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Meteor::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::GameEvents;
using namespace Games::Schedulers::Events::SystemEvents;
using namespace Instruments;
using namespace Meteor::Schedulers::Events::IoEvents;
using namespace Meteor::Schedulers::Events::InstrumentEvents;
using namespace Meteor::Schedulers::Events::PlayfieldEvents;
using namespace Meteor::Schedulers::Events::TimeEvents;
//using namespace Meteor::Schedulers::Events::RecorderEvents;


int MeteorPatternGenerator::load()
{

	MeteorConfigManager * m = GetCache<MeteorConfigManager>("MeteorConfigManager");

	if (!m)
		throw runtime_error("int MeteorPatternGenerator::load() : MeteorConfigManager not found in cache.");

	return load(m);
}

int MeteorPatternGenerator::load(MeteorConfigManager * mcf)
{
	// TODO: throw error
	if( !mcf->Get(MeteorSetting::FallSpeed, &fallSpeed))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : FallSpeed not found in Setting.");

	if( !mcf->Get(MeteorSetting::FallBrightness, &fallBrightness))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : FallBrightness not found in Setting.");

	if (!mcf->Get(MeteorSetting::FallLength, &fallLength))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : FallLength not found in Setting.");

	if( !mcf->Get(MeteorSetting::ExplodeSpeed, &explodeSpeed))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : ExplodeSpeed not found in Setting.");

	if( !mcf->Get(MeteorSetting::ExplodeBrightness, &explodeBrightness))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : ExplodeBrightness not found in Setting.");

	if( !mcf->Get(MeteorSetting::GlowLineSpeed, &glowLineSpeed))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : GlowLineSpeed not found in Setting.");

	if( !mcf->Get(MeteorSetting::GlowLineDuration, &glowLineDuration))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : GlowLineDuration not found in Setting.");

	if( !mcf->Get(MeteorSetting::GlowLineBrightness, &glowLineBrightness))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : GlowLineBrightness not found in Setting.");

	if (!mcf->Get(MeteorSetting::TargetLineBlinkSpeed, &targetLineBlinkSpeed))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : TargetLineBlinkSpeed not found in Setting.");
	
	if (!mcf->Get(MeteorSetting::RestartSection, &restartSection))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : RestartSection not found in Setting.");


	if (!mcf->Get(MeteorSetting::WhiteKeyTargetHeight, &targetHeight))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : WhiteKeyTargetHeight not found in Setting.");

	if (!mcf->Get(MeteorSetting::BlackKeyTargetHeight, &blackKeyTargetHeight))
		throw runtime_error("int MeteorPatternGenerator::load(MeteorConfigManager*) : BlackKeyTargetHeight not found in Setting.");


	return 0;
}

MeteorPatternGenerator::MeteorPatternGenerator(): PatternGenerator(), RegisterType("MeteorPatternGenerator")
{
	registerLoad(bind(static_cast<int(MeteorPatternGenerator::*)(void)>(&MeteorPatternGenerator::load), this));
}

int MeteorPatternGenerator::Initialize(Sm<Event>* s)
{
	PatternGenerator::Initialize(s);

	fallSpeed = s->GetSmInfo()->difficuty->Speed;

	return 0;
}

Pattern* MeteorPatternGenerator::Generate(vector<Event*>* es, Event * e)
{
	
	/* 所有的event都要複製一個新的來用，才不會動到原本讀好的譜 */
	/* 為什麼沒有delete?? 是不是之後要拿來當判斷點?? */
	//Event* eventClone = e->Clone();
	NoteControlPoint* note = e->Cast<NoteControlPoint>(); //eventClone->Cast<NoteControlPoint>();
	StartGameEvent* start = e->Cast<StartGameEvent>(); //eventClone->Cast<StartGameEvent>();
	InputKeyControlPoint* inputKey = e->Cast<InputKeyControlPoint>();
	SectionStartControlPoint* sectionStart = e->Cast<SectionStartControlPoint>();

	LOG(LogLevel::Depricated) << "MeteorPatternGenerator::Generate() : generate event [" << e->GetTypeName() << "] at [" << e->GetStartTime() << "].";

	/* 鍵力音符的特效 */
	if (note) {
		return generateNoteControlPoint(es, note);
	}

	/* 鍵力開始的特效 */ /* (這個功能已經沒有再用了，因為target line被拿掉了) */
	if (start) {
		return generateStartGameEvent(es, start);
	}

	/* 鍵力踏板的特效 */
	if (inputKey) {
		return generateInputKeyControlPoint(es, inputKey);
	}

	if (sectionStart) {
		LOG(LogLevel::Depricated) << "MeteorPatternGenerator::Generate() : generate repeat practice event." << sectionStart->GetStartTime();
		return generateRepeatPracticeEvent(es, sectionStart);
	}

	if (!note && !start && !inputKey && !sectionStart)
		throw runtime_error("Pattern* MeteorPatternGenerator::Generate(vector<Event*>*, Event*) : event cannot cast to any control point.");


	return nullptr;

}

int MeteorPatternGenerator::CreateOtherEvent(vector<Event*>* es)
{
	/* 這段程式碼重寫 */

	LOG(LogLevel::Fine) << "MeteorPatternGenerator::CreateOtherEvent() : sorting the events.";
	/* 將目前所有Event排序 */
	sort(es->begin(), es->end(),
		[](Event* const& a, Event* const& b) {

		if (a->GetStartTime() == b->GetStartTime())
			if (a->Cast<MarkControlPoint>() && b->Cast<MarkControlPoint>())
				return a->Cast<MarkControlPoint>()->GetSectionIndex() < b->Cast<MarkControlPoint>()->GetSectionIndex();

		return a->GetStartTime() < b->GetStartTime();
	});

	/*
	 *	一開始先檢查這個譜有沒有小節資訊，沒有小節資訊才要加小節事件。譜裡面沒有小節資訊的話，就以5秒為一小節
	 */
	
	if (!sm->GetSmInfo()->hasSectionData) {
		LOG(LogLevel::Warning) << "MeteorPatternGenerator::CreateOtherEvent() : no section data.";

		/* 先檢查是不是有小節資料，但是忘記標註HasSection，忘記標註的話就直接退出 */
		for (int i = 0; i < es->size(); i++) {
			if (es->at(i)->Cast<MarkControlPoint>()) {
				if (es->at(i)->Cast<MarkControlPoint>()->GetSectionIndex() > 0) {
					return 0;
				}
			}
		}

		vector<float> sectionStartTime;

		/* 把每個event的小節數用時間回推出來，然後再設定進去 */
		int maxSection = 0;
		//float maxControlPointTime = 0;

		for (int i = 0; i < es->size(); i++) {

			float startTime = 0;
			// 如果不適MarkControlPoint就不用設定section index
			//if (es->at(i)->GetSourceEvent() != nullptr) {
			//	startTime = es->at(i)->GetSourceEvent()->GetStartTime();
			//}
			//else {
			//	startTime = es->at(i)->GetStartTime();
			//}

			if (es->at(i)->Cast<MarkControlPoint>()) {

				startTime = es->at(i)->GetStartTime();

				int tempSection = floor(startTime / defaultSectionInterval);

				es->at(i)->Cast<MarkControlPoint>()->SetSectionIndex(tempSection);
				LOG(LogLevel::Debug) << "MeteorPatternGenerator::CreateOtherEvent() : [" << es->at(i)->GetTypeName() << "] at section [" << es->at(i)->Cast<MarkControlPoint>()->GetSectionIndex() << "]." << es->at(i);

				if (tempSection > maxSection)
					maxSection = tempSection;
			}

			/*
			if (es->at(i)->GetStartTime() < (tempSection + 1) * defaultSectionInterval) {
				
				if(es->at(i)->Cast<MarkControlPoint>())
					es->at(i)->Cast<MarkControlPoint>()->SetSectionIndex(tempSection);

				bool eventNotSortedError = false;
				if (es->at(i)->GetSourceEvent() != nullptr) {
					if (es->at(i)->GetSourceEvent()->GetStartTime() >= maxControlPointTime) {
						maxControlPointTime = es->at(i)->GetSourceEvent()->GetStartTime();
					}
					else {
						eventNotSortedError = true;
					}
				}
				else {
					if (es->at(i)->GetStartTime() >= maxControlPointTime) {
						maxControlPointTime = es->at(i)->GetStartTime();
					}
					else {
						eventNotSortedError = true;
					}
				}

				if (eventNotSortedError) {
					LOG(LogLevel::Error) << "MeteorPatternGenerator::CreateOtherEvent() : events not sorted by time." << es->at(i)->GetStartTime() << es->at(i)->GetTypeName();
					throw runtime_error("MeteorPatternGenerator::CreateOtherEvent() : events not sorted by time.");
				}
				
			}
			else {
				tempSection++;
				i--;
			}
			*/
		}


		for (int i = 0; i < maxSection + 1; i++) {
			sectionStartTime.push_back(i * defaultSectionInterval);
		}

		generateRepeatPracticeEvents(es, &sectionStartTime);

		/* 將目前所有Event排序 */
		sort(es->begin(), es->end(),
			[](Event* const& a, Event* const& b) {

			if (a->GetStartTime() == b->GetStartTime())
				if (a->Cast<MarkControlPoint>() && b->Cast<MarkControlPoint>())
					return a->Cast<MarkControlPoint>()->GetSectionIndex() < b->Cast<MarkControlPoint>()->GetSectionIndex();

			return a->GetStartTime() < b->GetStartTime();
		});

	}
	
	return 0;

	// --------------------------------------------後面的code 不用了--------------------------------------------

	/*
	 *	這邊會抓所有section結束時間，然後在每個section結束時間建repeat practice event，
	 *	另外meteor ruleset executor自己就會檢查一遍所有control point，然後建立所有的section start time，所以其實他不用看我們這邊的rewind length
	 *	不過還是把rewind length紀錄一下留著用
	 */

	 /*
	  *	一開始先檢查這個譜有沒有小節資訊
	  */

	/*
	LOG(LogLevel::Fine) << "MeteorPatternGenerator::CreateOtherEvent() : checking if this sheetmusic has section index";
	bool hasSectionIndex = true;
	for (int i = 0; i < es->size(); i++) {
		if (es->at(i)->Cast<PlayableControlPoint>() == nullptr)
			continue;

		if (es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() < 0)
			hasSectionIndex = false;

		if (!hasSectionIndex && es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() >= 0) {
			LOG(LogLevel::Error) << "MeteorPatternGenerator::CreateOtherEvent() : sheetmusic section index wrong at time [" << es->at(i - 1)->Cast<PlayableControlPoint>()->GetStartTime() << "]";
			throw runtime_error("MeteorPatternGenerator::CreateOtherEvent() : sheetmusic section index wrong.");
		}
	}

	LOG(LogLevel::Fine) << "MeteorPatternGenerator::CreateOtherEvent() : sorting the events.";
	// 排序
	sort(es->begin(), es->end(),
		[](Event* const& a, Event* const& b) { 
		
		if (a->GetStartTime() == b->GetStartTime())
			if(a->Cast<PlayableControlPoint>() && b->Cast<PlayableControlPoint>())
			return a->Cast<PlayableControlPoint>()->GetSectionIndex() < b->Cast<PlayableControlPoint>()->GetSectionIndex();
		
		return a->GetStartTime() < b->GetStartTime(); 
	});


	LOG(LogLevel::Fine) << "MeteorPatternGenerator::CreateOtherEvent() : start insert repeat event.";
	

	if (hasSectionIndex) {	// 譜裡面有小節資訊
		int tempSection = 0;
		float minEventStartTime = 99999;

		for (int i = 0; i < es->size(); i++) {
			if (es->at(i)->Cast<PlayableControlPoint>()) {	// section index其實不該擺在playable control point，應該要擺在她的上一層，因為不識只有playable control point會有section

				//if(es->at(i)->Cast<NoteControlPoint>())
				//	LOG(LogLevel::Debug) << "MeteorPatternGenerator::CreateOtherEvent() : counting [" << int(es->at(i)->Cast<NoteControlPoint>()->GetPitch()) << "] time [" << es->at(i)->GetStartTime() << "] section [" << es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() << "] .";
				//if (es->at(i)->Cast<InputKeyControlPoint>())
				//	LOG(LogLevel::Debug) << "MeteorPatternGenerator::CreateOtherEvent() : counting [Pedal] time [" << es->at(i)->GetStartTime() << "] section [" << es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() << "] .";


				if (es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() == tempSection + 1) {
					// 下個小節的第一個音就是這個小節的結束點
					sectionEndTime.push_back(es->at(i)->GetStartTime());
					tempSection++;
				}
				else if (es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() > tempSection + 1) {
					// 空白小節的時候就先把目前秒數加上去section end time，然後再重新檢查一次同一個音
					// 這邊之後在repeat的時候要小心，因為有兩個小節同時在同一個瞬間，會出錯
					sectionEndTime.push_back(es->at(i)->GetStartTime());
					tempSection++;
					i--;
				}
				else if (es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() < tempSection) {
					// 沒有照時間排序，要不然就是譜有錯
					LOG(LogLevel::Error) << "MeteorPatternGenerator::CreateOtherEvent() : events not sorted by time. temp Section:[" << tempSection << "], error event[" << es->at(i)->GetTypeName() << "] [" << es->at(i)->GetStartTime() << "] ,section [" << es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() << "].";
					throw runtime_error("MeteorPatternGenerator::CreateOtherEvent() : events not sorted by time.");
				}
			}
		}
	}
	else {	// 譜裡面沒有小節資訊的話，就以3秒為一小節

		int tempSection = 0;
		float maxControlPointTime = 0;

		for (int i = 0; i < es->size(); i++) {
			if (es->at(i)->GetStartTime() < (tempSection + 1) * defaultSectionInterval) {
				es->at(i)->Cast<PlayableControlPoint>()->SetSectionIndex(tempSection);
				if (es->at(i)->GetStartTime() >= maxControlPointTime) {
					maxControlPointTime = es->at(i)->GetStartTime();
				}
				else {
					LOG(LogLevel::Error) << "MeteorPatternGenerator::CreateOtherEvent() : events not sorted by time.";
					throw runtime_error("MeteorPatternGenerator::CreateOtherEvent() : events not sorted by time.");
				}
			}
			else {
				tempSection++;
				i--;
			}
		}

		for (int i = 0; i < tempSection; i++) {
			sectionEndTime.push_back((i + 1) * defaultSectionInterval);
		}
	}

	*/


	/*
	if (restartSection == 0)
		return -1;

	MTO_FLOAT sectionStartTime = 99999;
	Pattern* sectionFirstPattern = NULL;

	for (int i = 0; i < patterns.size(); i++) {
		Event* e = patterns.at(i)->GetOriginalEvent();
		
		if (e)
		if (e->Cast<NoteControlPoint>())
		if (e->Cast<NoteControlPoint>()->GetSectionIndex() == restartSection)
		if (e->Cast<NoteControlPoint>()->GetStartTime() < sectionStartTime) {
			
			sectionFirstPattern = patterns.at(i);
			sectionStartTime = e->Cast<NoteControlPoint>()->GetStartTime();

		}
	}

	SystemEvent* systemEvent = new StopSystemEvent(sectionStartTime, -1);

	es->push_back(systemEvent);
	*/


	return 0;
}

int MeteorPatternGenerator::PostProcess()
{
	for (int i = 0; i < patterns.size(); i++) {



	}


	return 0;
}

Pattern * MeteorPatternGenerator::generateNoteControlPoint(vector<Event*>* es, NoteControlPoint * note)
{
	NoteControlPoint * cloned = dynamic_cast<NoteControlPoint*>(note->Clone());
	/* 在pattern generator消滅實消滅，或是converter跑完消滅 */
	Pattern* pattern = new Pattern(cloned);

	LOG(LogLevel::Finer) << "int MeteorSmConverter::generateNoteControlPoint(vector<Event*>*, Event*) : Start converting [" << static_cast<int>(cloned->GetPitch()) << "," << note->GetStartTime() << "] to pattern...";

	/* 如果這個音的狀態是隱藏，就直接返回 */
	if(static_cast<int>(cloned->GetHandType()) < 0)
		return pattern;
	
	Pitch pitch = cloned->GetPitch();

	if (static_cast<int>(pitch) > static_cast<int>(startPitch) + width + extendWidth - 1 ||
		static_cast<int>(pitch) < static_cast<int>(startPitch) - extendWidth) {
		LOG(LogLevel::Warning) << "int MeteorSmConverter::generateNoteControlPoint(vector<Event*>*, Event*) : Note [" << static_cast<int>(cloned->GetPitch()) << " is out of light map boundary.";
		return pattern;
	}

	float volume = cloned->GetVolume() <= 0 || cloned->GetVolume() > 1 ? defaultVolume : cloned->GetVolume();



	// 公式： (鍵盤高度-打擊點高度) / 速度
	MTO_FLOAT fallTime = MTO_FLOAT(
		cloned->IsWhiteKey() ?
		targetHeight : blackKeyTargetHeight
	) / fallSpeed;

	MTO_FLOAT fallLifeTime = MTO_FLOAT(
		(cloned->IsWhiteKey() ?
			height : blackKeyHeight) + fallLength
	) / fallSpeed;


	LOG(LogLevel::Depricated) << "int MeteorSmConverter::Generate(vector<Event*>*, Event*) : Generate Fall at [" << (int)pitch << "][" << cloned->GetStartTime() << "], start time [" << cloned->GetStartTime() - fallTime << "], life time [" << fallTime << "].";

	FallEffect* fall = new FallEffect(
		int(pitch),
		0,
		cloned->GetStartTime() - fallTime,
		fallTime,
		fallSpeed);
	fall->SetTargetHeight(cloned->IsWhiteKey() ? targetHeight : blackKeyTargetHeight);
	fall->SetMeteorEffectShiftType(meteorEffectShiftType);
	fall->SetSourceEvent(cloned);

	EruptEffect* erupt = new EruptEffect(
		int(pitch),
		0,
		cloned->GetStartTime() - fallTime,
		fallLifeTime,
		fallSpeed
	);
	erupt->SetTargetHeight(cloned->IsWhiteKey() ? targetHeight : blackKeyTargetHeight);
	erupt->SetMeteorEffectShiftType(meteorEffectShiftType);
	erupt->SetSourceEvent(cloned);

	PianoSoundEvent* pianoSoundEventDown = new PianoSoundEvent(
		pair<Pitch, float>(pitch, volume),
		cloned->GetStartTime(),
		0
	);
	pianoSoundEventDown->SetSourceEvent(cloned);
	LOG(LogLevel::Depricated) << "int MeteorSmConverter::Generate(vector<Event*>*, Event*) : Generate sound at [" << (int)pitch << "], start time [" << cloned->GetStartTime() << "] volume [" << volume << "].";


	PianoSoundEvent* pianoSoundEventUp = nullptr;

	if (cloned->GetLifeTime() > 0) {
		pianoSoundEventUp = new PianoSoundEvent(
			pair<Pitch, float>(pitch, 0),
			cloned->GetStartTime() + cloned->GetLifeTime(),
			0
		);
	}
	else {
		// TODO: 這邊這樣寫可能播起來會很難聽，要設一下default life time
		// 要好聽可能要到post process的時候去計算小節來決定按下時間，但這個很難寫
		pianoSoundEventUp = new PianoSoundEvent(
			pair<Pitch, float>(pitch, 0),
			cloned->GetStartTime() + 0.1,
			0
		);
	}
	pianoSoundEventUp->SetSourceEvent(cloned);




	pattern->Add(fall);
	pattern->Add(erupt);
	pattern->Add(pianoSoundEventDown);
	pattern->Add(pianoSoundEventUp);

	// 把pattern裡面的event一個一個加進去es裡
	es->push_back(cloned);
	es->push_back(fall);
	es->push_back(erupt);
	es->push_back(pianoSoundEventDown);
	es->push_back(pianoSoundEventUp);

	return pattern;

}

Pattern * MeteorPatternGenerator::generateStartGameEvent(vector<Event*>* es, StartGameEvent * start)
{
	/* 在pattern generator消滅實消滅，或是converter跑完消滅 */
	Pattern* pattern = new Pattern();

	return pattern;

	LOG(LogLevel::Finer) << "int MeteorSmConverter::generateStartGameEvent(vector<Event*>*, StartGameEvent*) : Start converting [ StartGameEvent ," << start->GetStartTime() << "] to pattern...";


	LOG(LogLevel::Finer) << "int MeteorSmConverter::generateStartGameEvent(vector<Event*>*, StartGameEvent*) : Generate TargetLine at [" 
		<< targetHeight << "(white) , " << blackKeyTargetHeight << "(black) ], start time [" << start->GetStartTime()<< "], life time [ INFINITE ].";

	TargetLineEffect* targetLine = new TargetLineEffect(
		targetHeight,
		blackKeyTargetHeight,
		start->GetStartTime(),
		-1,	// 之後要去define每一個數是什麼意思
		targetLineBlinkSpeed);

	pattern->Add(targetLine);
	//pattern->Add(start);

	/* 把pattern裡面的event一個一個加進去es裡 */
	es->push_back(targetLine);

	return pattern;
}

Pattern * MeteorPatternGenerator::generateInputKeyControlPoint(vector<Event*>* es, InputKeyControlPoint * inputKeyControlPoint)
{
	InputKeyControlPoint* cloned = dynamic_cast<InputKeyControlPoint*>(inputKeyControlPoint->Clone());
	/* 在pattern generator消滅實消滅，或是converter跑完消滅 */
	Pattern* pattern = new Pattern(cloned);

	LOG(LogLevel::Finer) << "int MeteorSmConverter::generateNoteControlPoint(vector<Event*>*, Event*) : Start converting [" << static_cast<int>(cloned->GetInputKey()) << "," << cloned->GetStartTime() << "] to pattern...";

	/* 如果這個音的狀態是隱藏，就直接返回 */
	if (cloned->GetHandType() == HandType::Hidden ||
		cloned->GetHandType() == HandType::HiddenLeft ||
		cloned->GetHandType() == HandType::HiddenRight)
		return pattern;

	InputKey inputKey = cloned->GetInputKey();


	if (inputKey == InputKey::SustainPedal) {

		SustainPedalIoEvent* sustainPedalIoEvent = new SustainPedalIoEvent(inputKey, cloned->GetStartTime(), cloned->GetLifeTime());

		PianoEvent* pianoEventDown = new PianoEvent(pair<InputKey, int>(inputKey, 1), cloned->GetStartTime(), 0);
		PianoEvent* pianoEventUp = new PianoEvent(pair<InputKey, int>(inputKey, -1), cloned->GetStartTime() + cloned->GetLifeTime(), 0);

		PianoSoundEvent* pianoSoundEventDown = new PianoSoundEvent(true, cloned->GetStartTime(), 0);
		PianoSoundEvent* pianoSoundEventUp = new PianoSoundEvent(false, cloned->GetStartTime() + cloned->GetLifeTime(), 0);

		// recorder會自動去抓譜裡面的pedal，不用從這邊建event
		//MeteorButtonRecorderEvent* pedalDownRecorderEvent = new MeteorButtonRecorderEvent(pair<InputKey, int>(inputKey, 1), inputKeyControlPoint->GetStartTime(), 0);
		//MeteorButtonRecorderEvent* pedalUpRecorderEvent = new MeteorButtonRecorderEvent(pair<InputKey, int>(inputKey, 0), inputKeyControlPoint->GetStartTime() + inputKeyControlPoint->GetLifeTime() - 0.01, 0);


		LOG(LogLevel::Depricated) << "MeteorSmConverter::generateInputKeyControlPoint() : Piano Event [" << cloned->GetStartTime() << "].";
		
		sustainPedalIoEvent->SetSourceEvent(cloned);
		pianoEventDown->SetSourceEvent(cloned);
		pianoEventUp->SetSourceEvent(cloned);
		pianoSoundEventDown->SetSourceEvent(cloned);
		pianoSoundEventUp->SetSourceEvent(cloned);
		//pedalDownRecorderEvent->SetSourceEvent(inputKeyControlPoint);
		//pedalUpRecorderEvent->SetSourceEvent(inputKeyControlPoint);

		pattern->Add(sustainPedalIoEvent);
		pattern->Add(pianoEventDown);
		pattern->Add(pianoEventUp);
		pattern->Add(pianoSoundEventDown);
		pattern->Add(pianoSoundEventUp);
		//pattern->Add(pedalDownRecorderEvent);
		//pattern->Add(pedalUpRecorderEvent);

		es->push_back(cloned);
		es->push_back(sustainPedalIoEvent);
		es->push_back(pianoEventDown);
		es->push_back(pianoEventUp);
		es->push_back(pianoSoundEventDown);
		es->push_back(pianoSoundEventUp);
		//es->push_back(pedalDownRecorderEvent);
		//es->push_back(pedalUpRecorderEvent);
	}
	else if (inputKey == InputKey::LowerOctave || inputKey == InputKey::RaiseOctave) {

		OctaveShiftEvent* octaveShiftEvent = nullptr;
		
		if (inputKey == InputKey::LowerOctave) {
			octaveShiftEvent = new OctaveShiftEvent(OctaveShiftType::Lower, cloned->GetStartTime(), cloned->GetLifeTime());
			
			/* 切換目前的音域，用來改變effect出現的位置 */
			switch (meteorEffectShiftType) {
			case MeteorEffectShiftType::LoweredOctave:
				break;

			case MeteorEffectShiftType::None:
				meteorEffectShiftType = MeteorEffectShiftType::LoweredOctave;
				break;

			case MeteorEffectShiftType::RaisedOctave:
				meteorEffectShiftType = MeteorEffectShiftType::None;
				break;
			}
		}
		else {
			octaveShiftEvent = new OctaveShiftEvent(OctaveShiftType::Raise, cloned->GetStartTime(), cloned->GetLifeTime());

			/* 切換目前的音域，用來改變effect出現的位置 */
			switch (meteorEffectShiftType) {
			case MeteorEffectShiftType::LoweredOctave:
				meteorEffectShiftType = MeteorEffectShiftType::None;
				break;

			case MeteorEffectShiftType::None:
				meteorEffectShiftType = MeteorEffectShiftType::RaisedOctave;
				break;

			case MeteorEffectShiftType::RaisedOctave:
				break;
			}
		}

		// recorder會自動去抓譜裡面的pedal，不用從這邊建event
		//MeteorButtonRecorderEvent* buttonRecorderEvent = new MeteorButtonRecorderEvent(pair<InputKey, int>(inputKey, 1), inputKeyControlPoint->GetStartTime(), 0);

		PianoEvent* pianoEventPress = new PianoEvent(pair<InputKey, int>(inputKey, 1), cloned->GetStartTime(), 0);

		octaveShiftEvent->SetSourceEvent(cloned);
		//buttonRecorderEvent->SetSourceEvent(inputKeyControlPoint);
		pianoEventPress->SetSourceEvent(cloned);

		pattern->Add(octaveShiftEvent);
		//pattern->Add(buttonRecorderEvent);
		pattern->Add(pianoEventPress);

		es->push_back(cloned);
		es->push_back(octaveShiftEvent);
		//es->push_back(buttonRecorderEvent);
		es->push_back(pianoEventPress);
	}
	else {
		LOG(LogLevel::Warning) << "int MeteorSmConverter::generateInputKeyControlPoint(vector<Event*>*, Event*) : Input Key [" << static_cast<int>(inputKey) << " is not available.";
		return pattern;
	}


	return pattern;
}

int MeteorPatternGenerator::generateRepeatPracticeEvents(vector<Event*>* es, vector<float>* sectionStartTime)
{
	for (int i = 0; i < sectionStartTime->size() - 1; i++) {

		SectionStartControlPoint* sectionStartControlPoint = new SectionStartControlPoint(i, sectionStartTime->at(i), sectionStartTime->at(i + 1) - sectionStartTime->at(i));

		RepeatPracticeEvent* repeatPracticeEvent = new RepeatPracticeEvent(i, sectionStartTime->at(i + 1) - sectionStartTime->at(i), sectionStartTime->at(i + 1), 0);
		repeatPracticeEvent->SetSourceEvent(sectionStartControlPoint);

		es->push_back(sectionStartControlPoint);
		es->push_back(repeatPracticeEvent);
	}

	return 0;
}

Pattern* MeteorPatternGenerator::generateRepeatPracticeEvent(vector<Event*>* es, SectionStartControlPoint * sectionStart)
{
	SectionStartControlPoint* cloned = dynamic_cast<SectionStartControlPoint*>(sectionStart->Clone());
	LOG(LogLevel::Depricated) << "MeteorSmConverter::generateRepeatPracticeEvent() : generate repeat practice event on [" << cloned->GetSectionIndex() << "] section at [" << cloned->GetStartTime() << "].";
	Pattern* pattern = new Pattern(cloned);

	/* 直接用sectionStart->GetStartTime() + sectionStart->GetLifeTime()的話會被error說invalid use of non-static member function */
	float rewindLength = cloned->GetLifeTime();
	float repeatPracticeLifeTime = cloned->GetStartTime() + cloned->GetLifeTime();

	RepeatPracticeEvent* repeatPracticeEvent = new RepeatPracticeEvent(
		cloned->GetSectionIndex(),
		rewindLength,
		repeatPracticeLifeTime,
		0);

	repeatPracticeEvent->SetSourceEvent(cloned);

	pattern->Add(repeatPracticeEvent);
	es->push_back(cloned);
	es->push_back(repeatPracticeEvent);

	return pattern;
}

