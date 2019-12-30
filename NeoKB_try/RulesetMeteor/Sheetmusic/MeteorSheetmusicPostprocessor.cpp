#include "MeteorSheetmusicPostProcessor.h"

#include "../Scheduler/Event/IoEvents/SustainPedalIoEvent.h"


using namespace Meteor::Config;
using namespace Meteor::Sheetmusics;
using namespace Meteor::Schedulers::Events::IoEvents;


MeteorSmPostprocessor::MeteorSmPostprocessor() 
{
}

Sm<Event>* MeteorSmPostprocessor::postprocess(Sm<Event>* s)
{
	sort(s->GetEvents()->begin(), s->GetEvents()->end(),
		[](Event* const& a, Event* const& b) { return a->GetStartTime() < b->GetStartTime(); });

	SustainPedalIoEvent* lastSustainPedalIoEvent = nullptr;

	for (int i = 0; i < s->GetEvents()->size(); i++) {

		if (dynamic_cast<SustainPedalIoEvent*>(s->GetEvents()->at(i))) {

			SustainPedalIoEvent* sustainPedalIoEvent = dynamic_cast<SustainPedalIoEvent*>(s->GetEvents()->at(i));
			SustainPedalIoEvent* nextSustainPedalIoEvent = nullptr;

			for (int j = i; j < s->GetEvents()->size(); j++) {
				if (dynamic_cast<SustainPedalIoEvent*>(s->GetEvents()->at(j))) {
					nextSustainPedalIoEvent = dynamic_cast<SustainPedalIoEvent*>(s->GetEvents()->at(j));
					break;
				}
			}

			// �p�G���U�@��pedal�A�N�H�U�@��pedal���ɶ���ringLifeTime�C�����ɭ�next target���O��n���b�o��pedal����A���������j�@�q�ɶ�
			// ���N���n�ޤU�@��pedal�O�h�[�H��A�����H�{�b��pedal�񧹪��ɶ�����ringLifeTime�A
			MTO_FLOAT ringLifeTime = nextSustainPedalIoEvent == nullptr ? 
				sustainPedalIoEvent->GetLifeTime() : nextSustainPedalIoEvent->GetStartTime() - sustainPedalIoEvent->GetStartTime();

			if (ringLifeTime > sustainPedalIoEvent->GetLifeTime() + defaultNextPedalTargetStartTime * 2.f)
				ringLifeTime = sustainPedalIoEvent->GetLifeTime();

			/* �Ĥ@��pedal�� */
			if (lastSustainPedalIoEvent == nullptr) {

				sustainPedalIoEvent->SetTargetTime(
					sustainPedalIoEvent->GetStartTime() - defaultNextPedalTargetStartTime,
					defaultNextPedalTargetStartTime,
					sustainPedalIoEvent->GetLifeTime() - pedalInterval,
					ringLifeTime
				);

				

			}
			/* ��2~n��pedal */
			else {

				// �p�G�W�@��pedal�S����o�ӳs�򪺸ܡA���N���n��target start time�]�b�W�@��pedal�������ɶ��A�令�]�b�o��pedal�ֶ}�l���ɭԡC
				MTO_FLOAT targetStartTime = lastSustainPedalIoEvent->GetStartTime();
				MTO_FLOAT targetLifeTime = lastSustainPedalIoEvent->GetLifeTime();
				if (targetStartTime + lastSustainPedalIoEvent->GetLifeTime() < sustainPedalIoEvent->GetStartTime() - defaultNextPedalTargetStartTime * 2.f) {
					targetStartTime = sustainPedalIoEvent->GetStartTime() - defaultNextPedalTargetStartTime;
					targetLifeTime = defaultNextPedalTargetStartTime;
				}



				sustainPedalIoEvent->SetTargetTime(
					targetStartTime,
					targetLifeTime,
					sustainPedalIoEvent->GetLifeTime() - pedalInterval,
					ringLifeTime
				);

			}
			LOG(LogLevel::Debug) << "MeteorSmPostprocessor::postprocess() : sustainPedalIoEvent: [" << sustainPedalIoEvent->GetTargetStartTime() << "," << sustainPedalIoEvent->GetTargetLifeTime()
				<< "," << sustainPedalIoEvent->GetPedalDownLifeTime() << "," << sustainPedalIoEvent->GetRingLifeTime() << "]";
		}

	}


	return SmPostprocessor::postprocess(s);
}
