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

int MeteorPatternGenerator::Initialize(Sm<Event>* s)
{
	PatternGenerator::Initialize(s);

	fallSpeed = s->GetSmInfo()->difficuty->Speed;

	return 0;
}

Pattern* MeteorPatternGenerator::Generate(vector<Event*>* es, Event * e)
{
	
	/* �Ҧ���event���n�ƻs�@�ӷs���ӥΡA�~���|�ʨ�쥻Ū�n���� */
	/* ������S��delete?? �O���O����n���ӷ�P�_�I?? */
	//Event* eventClone = e->Clone();
	NoteControlPoint* note = e->Cast<NoteControlPoint>(); //eventClone->Cast<NoteControlPoint>();
	StartGameEvent* start = e->Cast<StartGameEvent>(); //eventClone->Cast<StartGameEvent>();
	InputKeyControlPoint* inputKey = e->Cast<InputKeyControlPoint>();
	SectionStartControlPoint* sectionStart = e->Cast<SectionStartControlPoint>();


	/* ��O���Ū��S�� */
	if (note)
		return generateNoteControlPoint(es, note);

	/* ��O�}�l���S�� */ /* (�o�ӥ\��w�g�S���A�ΤF�A�]��target line�Q�����F) */
	if (start)
		return generateStartGameEvent(es, start);

	/* ��O��O���S�� */
	if (inputKey)
		return generateInputKeyControlPoint(es, inputKey);

	if (sectionStart)
		return generateRepeatPracticeEvent(es, sectionStart);

	if (!note && !start && !inputKey && !sectionStart)
		throw runtime_error("Pattern* MeteorPatternGenerator::Generate(vector<Event*>*, Event*) : event cannot cast to any control point.");


	return nullptr;

}

int MeteorPatternGenerator::CreateOtherEvent(vector<Event*>* es)
{
	/* �o�q�{���X���g */

	LOG(LogLevel::Fine) << "MeteorPatternGenerator::CreateOtherEvent() : sorting the events.";
	/* �N�ثe�Ҧ�Event�Ƨ� */
	sort(es->begin(), es->end(),
		[](Event* const& a, Event* const& b) {

		if (a->GetStartTime() == b->GetStartTime())
			if (a->Cast<PlayableControlPoint>() && b->Cast<PlayableControlPoint>())
				return a->Cast<PlayableControlPoint>()->GetSectionIndex() < b->Cast<PlayableControlPoint>()->GetSectionIndex();

		return a->GetStartTime() < b->GetStartTime();
	});

	/*
	 *	�@�}�l���ˬd�o���Ц��S���p�`��T�A�S���p�`��T�~�n�[�p�`�ƥ�C�и̭��S���p�`��T���ܡA�N�H5���@�p�`
	 */
	
	if (!sm->GetSmInfo()->hasSectionData) {
		vector<float> sectionEndTime;

		int tempSection = 0;
		float maxControlPointTime = -5;

		for (int i = 0; i < es->size(); i++) {
			if (es->at(i)->GetStartTime() < (tempSection + 1) * defaultSectionInterval) {
				
				if(es->at(i)->Cast<MarkControlPoint>())
					es->at(i)->Cast<MarkControlPoint>()->SetSectionIndex(tempSection);

				if (es->at(i)->GetStartTime() >= maxControlPointTime) {
					maxControlPointTime = es->at(i)->GetStartTime();
				}
				else {
					LOG(LogLevel::Error) << "MeteorPatternGenerator::CreateOtherEvent() : events not sorted by time." << es->at(i)->GetStartTime();
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

		generateRepeatPracticeEvents(es, &sectionEndTime);
	}
	
	return 0;

	// --------------------------------------------�᭱��code ���ΤF--------------------------------------------

	/*
	 *	�o��|��Ҧ�section�����ɶ��A�M��b�C��section�����ɶ���repeat practice event�A
	 *	�t�~meteor ruleset executor�ۤv�N�|�ˬd�@�M�Ҧ�control point�A�M��إߩҦ���section start time�A�ҥH���L���άݧڭ̳o�䪺rewind length
	 *	���L�٬O��rewind length�����@�U�d�ۥ�
	 */

	 /*
	  *	�@�}�l���ˬd�o���Ц��S���p�`��T
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
	// �Ƨ�
	sort(es->begin(), es->end(),
		[](Event* const& a, Event* const& b) { 
		
		if (a->GetStartTime() == b->GetStartTime())
			if(a->Cast<PlayableControlPoint>() && b->Cast<PlayableControlPoint>())
			return a->Cast<PlayableControlPoint>()->GetSectionIndex() < b->Cast<PlayableControlPoint>()->GetSectionIndex();
		
		return a->GetStartTime() < b->GetStartTime(); 
	});


	LOG(LogLevel::Fine) << "MeteorPatternGenerator::CreateOtherEvent() : start insert repeat event.";
	

	if (hasSectionIndex) {	// �и̭����p�`��T
		int tempSection = 0;
		float minEventStartTime = 99999;

		for (int i = 0; i < es->size(); i++) {
			if (es->at(i)->Cast<PlayableControlPoint>()) {	// section index��ꤣ���\�bplayable control point�A���ӭn�\�b�o���W�@�h�A�]�����ѥu��playable control point�|��section

				//if(es->at(i)->Cast<NoteControlPoint>())
				//	LOG(LogLevel::Debug) << "MeteorPatternGenerator::CreateOtherEvent() : counting [" << int(es->at(i)->Cast<NoteControlPoint>()->GetPitch()) << "] time [" << es->at(i)->GetStartTime() << "] section [" << es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() << "] .";
				//if (es->at(i)->Cast<InputKeyControlPoint>())
				//	LOG(LogLevel::Debug) << "MeteorPatternGenerator::CreateOtherEvent() : counting [Pedal] time [" << es->at(i)->GetStartTime() << "] section [" << es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() << "] .";


				if (es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() == tempSection + 1) {
					// �U�Ӥp�`���Ĥ@�ӭ��N�O�o�Ӥp�`�������I
					sectionEndTime.push_back(es->at(i)->GetStartTime());
					tempSection++;
				}
				else if (es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() > tempSection + 1) {
					// �ťդp�`���ɭԴN����ثe��ƥ[�W�hsection end time�A�M��A���s�ˬd�@���P�@�ӭ�
					// �o�䤧��brepeat���ɭԭn�p�ߡA�]������Ӥp�`�P�ɦb�P�@�������A�|�X��
					sectionEndTime.push_back(es->at(i)->GetStartTime());
					tempSection++;
					i--;
				}
				else if (es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() < tempSection) {
					// �S���Ӯɶ��ƧǡA�n���M�N�O�Ц���
					LOG(LogLevel::Error) << "MeteorPatternGenerator::CreateOtherEvent() : events not sorted by time. temp Section:[" << tempSection << "], error event[" << es->at(i)->GetTypeName() << "] [" << es->at(i)->GetStartTime() << "] ,section [" << es->at(i)->Cast<PlayableControlPoint>()->GetSectionIndex() << "].";
					throw runtime_error("MeteorPatternGenerator::CreateOtherEvent() : events not sorted by time.");
				}
			}
		}
	}
	else {	// �и̭��S���p�`��T���ܡA�N�H3���@�p�`

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
	/* �bpattern generator����������A�άOconverter�]������ */
	Pattern* pattern = new Pattern(note);

	LOG(LogLevel::Finer) << "int MeteorSmConverter::generateNoteControlPoint(vector<Event*>*, Event*) : Start converting [" << static_cast<int>(note->GetPitch()) << "," << note->GetStartTime() << "] to pattern...";

	/* �p�G�o�ӭ������A�O���áA�N������^ */
	if(static_cast<int>(note->GetHandType()) < 0)
		return pattern;
	
	Pitch pitch = note->GetPitch();

	if (static_cast<int>(pitch) > static_cast<int>(startPitch) + width + extendWidth - 1 ||
		static_cast<int>(pitch) < static_cast<int>(startPitch) - extendWidth) {
		LOG(LogLevel::Warning) << "int MeteorSmConverter::generateNoteControlPoint(vector<Event*>*, Event*) : Note [" << static_cast<int>(note->GetPitch()) << " is out of light map boundary.";
		return pattern;
	}

	float volume = note->GetVolume() <= 0 || note->GetVolume() > 1 ? defaultVolume : note->GetVolume();



	// �����G (��L����-�����I����) / �t��
	MTO_FLOAT fallTime = MTO_FLOAT(
		note->IsWhiteKey() ?
		targetHeight : blackKeyTargetHeight
	) / fallSpeed;

	MTO_FLOAT fallLifeTime = MTO_FLOAT(
		(note->IsWhiteKey() ?
			height : blackKeyHeight) + fallLength
	) / fallSpeed;


	LOG(LogLevel::Finer) << "int MeteorSmConverter::Generate(vector<Event*>*, Event*) : Generate Fall at [" << (int)pitch << "], start time [" << note->GetStartTime() - fallTime << "], life time [" << fallLifeTime << "].";

	FallEffect* fall = new FallEffect(
		int(pitch),
		0,
		note->GetStartTime() - fallTime,
		fallLifeTime,
		fallSpeed);
	fall->SetTargetHeight(note->IsWhiteKey() ? targetHeight : blackKeyTargetHeight);
	fall->SetSourceEvent(note);

	EruptEffect* erupt = new EruptEffect(
		int(pitch),
		0,
		note->GetStartTime() - fallTime,
		fallLifeTime,
		fallSpeed
	);
	erupt->SetTargetHeight(note->IsWhiteKey() ? targetHeight : blackKeyTargetHeight);
	erupt->SetSourceEvent(note);

	PianoSoundEvent* pianoSoundEventDown = new PianoSoundEvent(
		pair<Pitch, float>(pitch, volume),
		note->GetStartTime(),
		0
	);
	pianoSoundEventDown->SetSourceEvent(note);
	LOG(LogLevel::Depricated) << "int MeteorSmConverter::Generate(vector<Event*>*, Event*) : Generate sound at [" << (int)pitch << "], start time [" << note->GetStartTime() << "] volume [" << volume << "].";


	PianoSoundEvent* pianoSoundEventUp = nullptr;

	if (note->GetLifeTime() > 0) {
		pianoSoundEventUp = new PianoSoundEvent(
			pair<Pitch, float>(pitch, 0),
			note->GetStartTime() + note->GetLifeTime(),
			0
		);
	}
	else {
		// TODO: �o��o�˼g�i�༽�_�ӷ|����ť�A�n�]�@�Udefault life time
		// �n�nť�i��n��post process���ɭԥh�p��p�`�ӨM�w���U�ɶ��A���o�ӫ����g
		pianoSoundEventUp = new PianoSoundEvent(
			pair<Pitch, float>(pitch, 0),
			note->GetStartTime() + 0.1,
			0
		);
	}
	pianoSoundEventUp->SetSourceEvent(note);




	pattern->Add(fall);
	pattern->Add(erupt);
	pattern->Add(pianoSoundEventDown);
	pattern->Add(pianoSoundEventUp);

	// ��pattern�̭���event�@�Ӥ@�ӥ[�i�hes��
	es->push_back(fall);
	es->push_back(erupt);
	es->push_back(pianoSoundEventDown);
	es->push_back(pianoSoundEventUp);

	return pattern;

}

Pattern * MeteorPatternGenerator::generateStartGameEvent(vector<Event*>* es, StartGameEvent * start)
{
	/* �bpattern generator����������A�άOconverter�]������ */
	Pattern* pattern = new Pattern();

	return pattern;

	LOG(LogLevel::Finer) << "int MeteorSmConverter::generateStartGameEvent(vector<Event*>*, StartGameEvent*) : Start converting [ StartGameEvent ," << start->GetStartTime() << "] to pattern...";


	LOG(LogLevel::Finer) << "int MeteorSmConverter::generateStartGameEvent(vector<Event*>*, StartGameEvent*) : Generate TargetLine at [" 
		<< targetHeight << "(white) , " << blackKeyTargetHeight << "(black) ], start time [" << start->GetStartTime()<< "], life time [ INFINITE ].";

	TargetLineEffect* targetLine = new TargetLineEffect(
		targetHeight,
		blackKeyTargetHeight,
		start->GetStartTime(),
		-1,	// ����n�hdefine�C�@�ӼƬO����N��
		targetLineBlinkSpeed);

	pattern->Add(targetLine);
	//pattern->Add(start);

	/* ��pattern�̭���event�@�Ӥ@�ӥ[�i�hes�� */
	es->push_back(targetLine);

	return pattern;
}

Pattern * MeteorPatternGenerator::generateInputKeyControlPoint(vector<Event*>* es, InputKeyControlPoint * inputKeyControlPoint)
{
	/* �bpattern generator����������A�άOconverter�]������ */
	Pattern* pattern = new Pattern(inputKeyControlPoint);

	LOG(LogLevel::Finer) << "int MeteorSmConverter::generateNoteControlPoint(vector<Event*>*, Event*) : Start converting [" << static_cast<int>(inputKeyControlPoint->GetInputKey()) << "," << inputKeyControlPoint->GetStartTime() << "] to pattern...";

	/* �p�G�o�ӭ������A�O���áA�N������^ */
	if (inputKeyControlPoint->GetHandType() == HandType::Hidden ||
		inputKeyControlPoint->GetHandType() == HandType::HiddenLeft || 
		inputKeyControlPoint->GetHandType() == HandType::HiddenRight)
		return pattern;

	InputKey inputKey = inputKeyControlPoint->GetInputKey();


	if (inputKey == InputKey::SustainPedal) {

		SustainPedalIoEvent* sustainPedalIoEvent = new SustainPedalIoEvent(inputKey, inputKeyControlPoint->GetStartTime(), inputKeyControlPoint->GetLifeTime());

		PianoEvent* pianoEventDown = new PianoEvent(pair<InputKey, int>(inputKey, 1), inputKeyControlPoint->GetStartTime(), 0);
		PianoEvent* pianoEventUp = new PianoEvent(pair<InputKey, int>(inputKey, -1), inputKeyControlPoint->GetStartTime() + inputKeyControlPoint->GetLifeTime() - 0.2, 0);

		PianoSoundEvent* pianoSoundEventDown = new PianoSoundEvent(true, inputKeyControlPoint->GetStartTime(), 0);
		PianoSoundEvent* pianoSoundEventUp = new PianoSoundEvent(false, inputKeyControlPoint->GetStartTime() + inputKeyControlPoint->GetLifeTime() - 0.1, 0);

		LOG(LogLevel::Depricated) << "MeteorSmConverter::generateInputKeyControlPoint() : Piano Event [" << inputKeyControlPoint->GetStartTime() << "].";
		
		sustainPedalIoEvent->SetSourceEvent(inputKeyControlPoint);
		pianoEventDown->SetSourceEvent(inputKeyControlPoint);
		pianoEventUp->SetSourceEvent(inputKeyControlPoint);
		pianoSoundEventDown->SetSourceEvent(inputKeyControlPoint);
		pianoSoundEventUp->SetSourceEvent(inputKeyControlPoint);

		pattern->Add(sustainPedalIoEvent);
		pattern->Add(pianoEventDown);
		pattern->Add(pianoEventUp);
		pattern->Add(pianoSoundEventDown);
		pattern->Add(pianoSoundEventUp);

		es->push_back(sustainPedalIoEvent);
		es->push_back(pianoEventDown);
		es->push_back(pianoEventUp);
		es->push_back(pianoSoundEventDown);
		es->push_back(pianoSoundEventUp);
	}
	else if (inputKey == InputKey::LowerOctave || inputKey == InputKey::RaiseOctave) {

		OctaveShiftEvent* octaveShiftEvent = nullptr;

		if (inputKey == InputKey::LowerOctave)
			octaveShiftEvent = new OctaveShiftEvent(OctaveShiftType::Lower, inputKeyControlPoint->GetStartTime(), inputKeyControlPoint->GetLifeTime());
		else
			octaveShiftEvent = new OctaveShiftEvent(OctaveShiftType::Raise, inputKeyControlPoint->GetStartTime(), inputKeyControlPoint->GetLifeTime());

		PianoEvent* pianoEventPress = new PianoEvent(pair<InputKey, int>(inputKey, 1), inputKeyControlPoint->GetStartTime(), 0);

		octaveShiftEvent->SetSourceEvent(inputKeyControlPoint);
		pianoEventPress->SetSourceEvent(inputKeyControlPoint);

		pattern->Add(octaveShiftEvent);
		pattern->Add(pianoEventPress);
		es->push_back(octaveShiftEvent);
		es->push_back(pianoEventPress);
	}
	else {
		LOG(LogLevel::Warning) << "int MeteorSmConverter::generateInputKeyControlPoint(vector<Event*>*, Event*) : Input Key [" << static_cast<int>(inputKey) << " is not available.";
		return pattern;
	}


	return pattern;
}

int MeteorPatternGenerator::generateRepeatPracticeEvents(vector<Event*>* es, vector<float>* sectionEndTime)
{
	for (int i = 0; i < sectionEndTime->size(); i++) {
		float rewindLength = i == 0 ? sectionEndTime->at(i) : sectionEndTime->at(i) - sectionEndTime->at(i - 1);
		RepeatPracticeEvent* repeatPracticeEvent = new RepeatPracticeEvent(i, rewindLength, sectionEndTime->at(i), 0);

		es->push_back(repeatPracticeEvent);
	}

	return 0;
}

Pattern* MeteorPatternGenerator::generateRepeatPracticeEvent(vector<Event*>* es, SectionStartControlPoint * sectionStart)
{
	Pattern* pattern = new Pattern(sectionStart);

	/* ������sectionStart->GetStartTime() + sectionStart->GetLifeTime()���ܷ|�Qerror��invalid use of non-static member function */
	float rewindLength = sectionStart->GetLifeTime();
	float repeatPracticeLifeTime = sectionStart->GetStartTime() + sectionStart->GetLifeTime();

	RepeatPracticeEvent* repeatPracticeEvent = new RepeatPracticeEvent(
		sectionStart->GetSectionIndex(), 
		rewindLength,
		repeatPracticeLifeTime,
		0);

	repeatPracticeEvent->SetSourceEvent(sectionStart);

	pattern->Add(repeatPracticeEvent);
	es->push_back(repeatPracticeEvent);

	return pattern;
}

