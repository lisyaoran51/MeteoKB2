#include "MeteorPatternGenerator.h"

#include "../../../Util/MtoType.h"
#include "../../../Base/Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../Scheduler/Event/Effect/FallEffect.h"
#include "../../Scheduler/Event/Effect/GlowLineEffect.h"
#include "../../Scheduler/Event/Effect/TargetLineEffect.h"
#include "../../../Base/Scheduler/Event/GameEvents/StartGameEvent.h"
#include "../../../Base/Scheduler/Event/SystemEvents/SystemEvent.h"
#include "../../../Base/Scheduler/Event/SystemEvents/StopSystemEvent.h"



using namespace Meteor::Sheetmusics::Patterns;
using namespace Meteor::Config;
using namespace Base::Schedulers::Events;
using namespace Base::Schedulers::Events::ControlPoints;
using namespace Meteor::Schedulers::Events::Effects;
using namespace Base::Schedulers::Events::GameEvents;
using namespace Base::Schedulers::Events::SystemEvents;



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


	return 0;
}

MeteorPatternGenerator::MeteorPatternGenerator(): PatternGenerator(), RegisterType("MeteorPatternGenerator")
{
	registerLoad(bind(static_cast<int(MeteorPatternGenerator::*)(void)>(&MeteorPatternGenerator::load), this));
}

Pattern* MeteorPatternGenerator::Generate(vector<Event*>* es, Event * e)
{
	
	/* 所有的event都要複製一個新的來用，才不會動到原本讀好的譜 */
	/* 為什麼沒有delete?? 是不是之後要拿來當判斷點?? */
	Event* eventClone = e->Clone();
	NoteControlPoint* note = eventClone->Cast<NoteControlPoint>();
	StartGameEvent* start = eventClone->Cast<StartGameEvent>();

	/* 鍵力音符的特效 */
	if (note)
		return generateNoteControlPoint(es, note);

	/* 鍵力開始的特效 */
	if (start)
		return generateStartGameEvent(es, start);


	if (!note && !start)
		throw runtime_error("Pattern* MeteorPatternGenerator::Generate(vector<Event*>*, Event*) : event cannot cast to NoteControlPoint or StartGameEvent.");


	return NULL;

}

int MeteorPatternGenerator::CreateOtherEvent(vector<Event*>* es)
{
	if (restartSection == 0)
		return -1;

	MTO_FLOAT sectionStartTime = 99999;
	Pattern* sectionFirstPattern = NULL;

	for (int i = 0; i < patterns.size(); i++) {
		Event* e = patterns.at(i)->GetOriginalEvent();
		
		/* 抓小節的開始時間 */
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

	return 0;
}

Pattern * MeteorPatternGenerator::generateNoteControlPoint(vector<Event*>* es, NoteControlPoint * note)
{
	/* 在pattern generator消滅實消滅，或是converter跑完消滅 */
	Pattern* pattern = new Pattern(note);

	LOG(LogLevel::Finer) << "int MeteorSmConverter::generateNoteControlPoint(vector<Event*>*, Event*) : Start converting [" << static_cast<int>(note->GetPitch()) << "," << note->GetStartTime() << "] to pattern...";

	/* 如果這個音的狀態是隱藏，就直接返回 */
	if(static_cast<int>(note->GetHandType()) > 1)
		return pattern;
	
	Pitch pitch = note->GetPitch();

	if (static_cast<int>(pitch) > static_cast<int>(startPitch) + width - 1 ||
		static_cast<int>(pitch) < static_cast<int>(startPitch)) {
		LOG(LogLevel::Warning) << "int MeteorSmConverter::generateNoteControlPoint(vector<Event*>*, Event*) : Note [" << static_cast<int>(note->GetPitch()) << " is out of light map boundary.";
		return pattern;
	}


	// 公式： (鍵盤高度-打擊點高度) / 速度
	MTO_FLOAT fallTime = MTO_FLOAT(
		note->IsWhiteKey() ?
		targetHeight : blackKeyTargetHeight
	) / fallSpeed;

	MTO_FLOAT fallLifeTime = MTO_FLOAT(
		(note->IsWhiteKey() ?
			height : blackKeyHeight) + fallLength
	) / fallSpeed;

	MTO_FLOAT glowLineTime = fallTime + MTO_FLOAT(1) / glowLineSpeed + glowLineDuration;

	MTO_FLOAT noteLifeTime = MTO_FLOAT(
		note->IsWhiteKey() ?
		(targetHeight) : (blackKeyTargetHeight)
	) / glowLineSpeed;

	LOG(LogLevel::Finest) << "int MeteorSmConverter::Generate(vector<Event*>*, Event*) : Fall speed is [" << fallSpeed << "], GlowLine speed is [" << glowLineSpeed << "].";

	LOG(LogLevel::Finer) << "int MeteorSmConverter::Generate(vector<Event*>*, Event*) : Generate GlowLine at [" << (int)pitch << "], start time [" << note->GetStartTime() - glowLineTime << "], life time [" << fallTime + glowLineDuration << "].";

	GlowLineEffect* glow = new GlowLineEffect(
		(int)pitch,
		0,
		note->GetStartTime() - glowLineTime,
		fallTime + glowLineDuration,
		glowLineSpeed);

	LOG(LogLevel::Finer) << "int MeteorSmConverter::Generate(vector<Event*>*, Event*) : Generate Fall at [" << (int)pitch << "], start time [" << note->GetStartTime() - fallTime << "], life time [" << fallLifeTime << "].";

	FallEffect* fall = new FallEffect(
		(int)pitch,
		0,
		note->GetStartTime() - fallTime,
		fallLifeTime,
		fallSpeed);

	note->SetLifeTime(noteLifeTime);

	pattern->Add(glow);
	pattern->Add(fall);
	//pattern->Add(note);

	// 把pattern裡面的event一個一個加進去es裡
	es->push_back(glow);
	es->push_back(fall);
	//es->push_back(note);

	return pattern;

}

Pattern * MeteorPatternGenerator::generateStartGameEvent(vector<Event*>* es, StartGameEvent * start)
{
	/* 在pattern generator消滅實消滅，或是converter跑完消滅 */
	Pattern* pattern = new Pattern();

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
