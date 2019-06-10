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

	/* �[�J�}�l�S�ġA�]�A�����u... */
	/* �o�ӳ����������ӭn�\��SmConverter�̭� */

	vector<Event*>* convertedEvent = new vector<Event*>();
	StartGameEvent* startGameEvent = new StartGameEvent(0, -1);

	Pattern* newPattern = patternGenerator->Generate(convertedEvent, startGameEvent);
	patternGenerator->Add(newPattern);

	for (int i = 0; i < convertedEvent->size(); i++) {
		convertedSm->GetEvents()->push_back(convertedEvent->at(i));
	}

	convertedEvent->clear();

	delete convertedEvent;

	/* [����] �[�J�}�l�S�ġA�]�A�����u... */

	return convertedSm;
}

int MeteorSmConverter::convertEvent(vector<Event*>* es, Event* e)
{

	//LOG(LogLevel::Info) << "int MeteorSmConverter::convertEvent(vector<Event*>*, Event*) : Start converting events ...";

	// �p�G�bŪ�ɪ��ɭԴN�O�B�z�n���ɮסA�N�����I���@�ӵM��^��
	// ���p�G�O�h�������p�A�S��k�o�˽ƻs�A�n��clone
	if (e->CanCast<Effect>()) {
		Event* effect = e->Clone();
		es->push_back(effect);
		return 0;
	}

	if (e->CanCast<NoteControlPoint>()) {
		// �糧�ӳ�ª����СA�ͦ��U�دS�ġA�p���ܥ��B���U�B��L�S�ġC

		Pattern* newPattern = patternGenerator->Generate(es, e);
		newPattern->SetOriginalEvent(e);

		patternGenerator->Add(newPattern);

	}

	return 0;
}
