#include"MeteorSheetmusicConverter.h"

#include "../../Base/Scheduler/Event/Effect/Effect.h"
#include "../../Base/Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../Base/Scheduler/Event/GameEvents/StartGameEvent.h"

using namespace Meteor::Sheetmusics;
using namespace Base::Schedulers::Events;
using namespace Base::Schedulers::Events::Effects;
using namespace Base::Schedulers::Events::ControlPoints;
using namespace Base::Schedulers::Events::GameEvents;



MeteorSmConverter::MeteorSmConverter(PatternGenerator * pg): SmConverter(pg)
{
}

Sm<Event>* MeteorSmConverter::Convert(Sm<Event>* s)
{
	LOG(LogLevel::Fine) << "MeteorSmConverter::Convert(Sm<Event>*) : Start converting sheetmusic...";
	Sm<Event>* convertedSm = SmConverter::Convert(s);

	/* 加入開始特效，包括打擊線... */
	/* 這個部分之後應該要擺到SmConverter裡面 */

	vector<Event*>* convertedEvent = new vector<Event*>();
	StartGameEvent* startGameEvent = new StartGameEvent(0, -1);

	Pattern* newPattern = patternGenerator->Generate(convertedEvent, startGameEvent);
	patternGenerator->Add(newPattern);

	for (int i = 0; i < convertedEvent->size(); i++) {
		convertedSm->GetEvents()->push_back(convertedEvent->at(i));
	}

	convertedEvent->clear();

	delete convertedEvent;

	/* [結束] 加入開始特效，包括打擊線... */

	return convertedSm;
}

int MeteorSmConverter::convertEvent(vector<Event*>* es, Event* e)
{

	//LOG(LogLevel::Info) << "int MeteorSmConverter::convertEvent(vector<Event*>*, Event*) : Start converting events ...";

	// 如果在讀檔的時候就是處理好的檔案，就直接付智一個然後回傳
	// 但如果是多型的狀況，沒辦法這樣複製，要用clone
	if (e->CanCast<Effect>()) {
		Event* effect = e->Clone();
		es->push_back(effect);
		return 0;
	}

	if (e->CanCast<NoteControlPoint>()) {
		// 對本來單純的樂譜，生成各種特效，如提示光、落下、其他特效。

		Pattern* newPattern = patternGenerator->Generate(es, e);
		newPattern->SetOriginalEvent(e);

		patternGenerator->Add(newPattern);

	}

	return 0;
}
