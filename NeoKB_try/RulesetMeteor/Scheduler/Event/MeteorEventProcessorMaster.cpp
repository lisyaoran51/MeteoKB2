#include "MeteorEventProcessorMaster.h"

#include "Effect/FallEffectMapper.h"
#include <utility>
#include "../../../Games/Scheduler/Event/ControlPoints/NoteControlPointHitObject.h"
#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Games/Scheduler/Event/InstrumentEvents/InstrumentEvent.h"
#include <sstream>
#include <iomanip>

// debug用
#include <chrono>
using namespace std::chrono;
// debug用



using namespace Meteor::Schedulers::Events;
using namespace Meteor::Schedulers::Events::Effects;
using namespace std;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Games::Output::Bluetooths;
using namespace Games::Schedulers::Events::InstrumentEvents;




int MeteorEventProcessorMaster::load()
{
	LOG(LogLevel::Depricated) << "MeteorEventProcessorMaster::load() : add hidden note effect filter.";
	eventProcessorFilter->AddFilterCallback(bind(&MeteorEventProcessorMaster::filterHiddenNoteEffects, this, placeholders::_1));


	return 0;
}

MeteorEventProcessorMaster::MeteorEventProcessorMaster() : RegisterType("MeteorEventProcessorMaster")
{
	registerLoad(bind((int(MeteorEventProcessorMaster::*)())&MeteorEventProcessorMaster::load, this));
}

int MeteorEventProcessorMaster::OnKeyDown(pair<MeteorAction, int> action)
{
	LOG(LogLevel::Depricated) << "MeteorEventProcessorMaster::OnKeyDown() : get input." << int(action.first);

	string gameEventContext = "PressKey,";

	stringstream stream;
	stream << fixed << setprecision(2) << GetClock()->GetCurrentTime();
	gameEventContext += stream.str();
	gameEventContext += string(",");
	gameEventContext += to_string(int(GetPitchFromAction(action.first)));
	gameEventContext += string(",");
	stream.clear();
	stream << fixed << setprecision(2) << float(action.second) / 128.0;
	gameEventContext += stream.str();

	MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

	json context;
	context["Events"].push_back(gameEventContext);

	meteoContextBluetoothMessage->SetContextInJson(context);
	meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

	outputManager->PushMessage(meteoContextBluetoothMessage);


	//double currentTime = GetClock()->GetCurrentTime();
	//
	//vector<EventProcessor<Event>*> eventProcessors;
	//
	//eventProcessorPeriods->GetItemsContainPeriods(make_pair<float, float>(currentTime - visibleTimeRange, currentTime + visibleTimeRange), &eventProcessors);
	//
	//eventProcessorFilter->Filter(&eventProcessors);

	if (!isGamePaused) {

		NoteControlPointHitObject* receivedHitObject = nullptr;

		for (int i = 0; i < filteredTempStaticEventProcessors.size(); i++) {

			NoteControlPointHitObject* noteControlPointHitObject = dynamic_cast<NoteControlPointHitObject*>(filteredTempStaticEventProcessors[i]);

			if (noteControlPointHitObject == nullptr)
				continue;

			LOG(LogLevel::Depricated) << "MeteorEventProcessorMaster::OnKeyDown() : checking hit object [" << (int)noteControlPointHitObject->GetPitch() << "] on [" << noteControlPointHitObject->GetStartTime() << "] matching [" << int(action.first) << "].";// on[" << currentTime << "].";

			if (!matchPitch(noteControlPointHitObject, action.first))
				continue;

			LOG(LogLevel::Depricated) << "MeteorEventProcessorMaster::OnKeyDown() : matched input! " << int(action.first);


			if (noteControlPointHitObject->GetHasJudgementResult())
				continue;

			LOG(LogLevel::Depricated) << "MeteorEventProcessorMaster::OnKeyDown() : not judged! " << int(action.first);

			if (noteControlPointHitObject->TryJudgement() > 0) {
				if (receivedHitObject != nullptr) {

					// 最晚的最先被打中，早的hit object就直接跳過
					// pedal事件先全部跳過，以後要檢查視不適game control pedal，是的話再跳過，game control pedal現在存在piano和meteo piano裡
					if (noteControlPointHitObject->TryJudgement() > receivedHitObject->TryJudgement())
						continue;
				}
				receivedHitObject = noteControlPointHitObject;
			}
		}

		if (receivedHitObject) {

			LOG(LogLevel::Debug) << "MeteorEventProcessorMaster::OnKeyDown() : find a hit object [" << receivedHitObject << "] on [" << int(action.first) << "].";
			receivedHitObject->UpdateJudgement(true);

		}
	}

	return 0;
}

int MeteorEventProcessorMaster::OnKeyUp(MeteorAction action)
{
	
	string gameEventContext = "ReleaseKey,";

	stringstream stream;
	stream << fixed << setprecision(2) << GetClock()->GetCurrentTime();
	gameEventContext += stream.str();
	gameEventContext += string(",");
	gameEventContext += to_string(int(GetPitchFromAction(action)));

	MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

	json context;
	context["Events"].push_back(gameEventContext);

	meteoContextBluetoothMessage->SetContextInJson(context);
	meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

	outputManager->PushMessage(meteoContextBluetoothMessage);


	return 0;
}

int MeteorEventProcessorMaster::OnButtonDown(MeteorAction action)
{
	if (action == MeteorAction::SustainPedal) {
		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

		string gameEventContext = "PedalDown,";

		stringstream stream;
		stream << fixed << setprecision(2) << GetClock()->GetCurrentTime();
		gameEventContext += stream.str();

		json context;
		context["Events"].push_back(gameEventContext);

		meteoContextBluetoothMessage->SetContextInJson(context);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);
	}

	return 0;
}

int MeteorEventProcessorMaster::OnButtonUp(MeteorAction action)
{
	if (action == MeteorAction::SustainPedal) {
		MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::HardwareGameEvent);

		string gameEventContext = "PedalUp,";

		stringstream stream;
		stream << fixed << setprecision(2) << GetClock()->GetCurrentTime();
		gameEventContext += stream.str();

		json context;
		context["Events"].push_back(gameEventContext);

		meteoContextBluetoothMessage->SetContextInJson(context);
		meteoContextBluetoothMessage->SetAccessType(MeteoBluetoothMessageAccessType::ReadOnly);

		outputManager->PushMessage(meteoContextBluetoothMessage);
	}

	return 0;
}

int MeteorEventProcessorMaster::OnKnobTurn(pair<MeteorAction, int> action)
{


	return 0;
}

int MeteorEventProcessorMaster::OnSlide(pair<MeteorAction, int> action)
{
	return 0;
}

int MeteorEventProcessorMaster::update()
{
	system_clock::time_point systemStartTime = system_clock::now();

	EventProcessorMaster::update();

	double currentTime = 0;
	/* 這邊要檢查已經過去的fall effect有沒有miss */
	try {
		currentTime = GetClock()->GetCurrentTime();
	}
	catch (exception& e) {
		LOG(LogLevel::Warning) << "EventProcessorMaster::GetGraph : clock is not started [" << e.what() << "].";
		return 0;
		//abort();
	}

	system_clock::time_point systemCurrentTime = system_clock::now();
	LOG(LogLevel::Finest) << "MeteorEventProcessorMaster::update() : [EventProcessorMaster] update cost time = [" << duration_cast<microseconds>(systemCurrentTime - systemStartTime).count() << "].";
	
	/*
	vector<EventProcessor<Event>*> eventProcessors;

	// 拿已經結束的event
	eventProcessorPeriods->GetItemsContainPeriods(make_pair<float, float>(currentTime - visibleTimeRange, (float)currentTime), &eventProcessors);
	LOG(LogLevel::Depricated) << "MeteorEventProcessorMaster::update() : filter event processors by [" << eventProcessorFilter << "].";

	systemStartTime = system_clock::now();
	LOG(LogLevel::Finest) << "MeteorEventProcessorMaster::update() : event processors [" << eventProcessors.size() << "].";
	eventProcessorFilter->Filter(&eventProcessors);

	systemCurrentTime = system_clock::now();
	LOG(LogLevel::Finest) << "MeteorEventProcessorMaster::update() : [eventProcessorFilter] update cost time = [" << duration_cast<microseconds>(systemCurrentTime - systemStartTime).count() << "].";
	*/

	LOG(LogLevel::Depricated) << "MeteorEventProcessorMaster::update() : filteredTempStaticEventProcessors size [" << filteredTempStaticEventProcessors.size() << "].";


	for (int i = 0; i < filteredTempStaticEventProcessors.size(); i++) {

		LOG(LogLevel::Depricated) << "MeteorEventProcessorMaster::update() : traverse to [" << filteredTempStaticEventProcessors[i]->GetEvent()->GetTypeName() << "] .";


		if (filteredTempStaticEventProcessors[i]->GetEventProcessorType() == EventProcessorType::ControlPoint) {

			HitObject* hObject = dynamic_cast<HitObject*>(filteredTempStaticEventProcessors[i]);
			if (hObject == nullptr)
				continue;

			if (hObject->GetHasJudgementResult())
				continue;

			if (hObject->TryJudgement() == -2) {
				// TODO: 目前先不檢查踏板，之後要改成如果有插入踏板就要檢查踏板
				// 應該永遠不檢查踏板，不考慮這個要素
				if (dynamic_cast<NoteControlPointHitObject*>(hObject)) {
					hObject->UpdateJudgement(false);
				}
			}
		}
		
	}

	return 0;
}

int MeteorEventProcessorMaster::LoadOnComplete()
{
	loadAndMapPitches();

	return 0;
}

int MeteorEventProcessorMaster::loadAndMapPitches()
{
	/* 未升降 */
#pragma region NonePitchState


	pitchBindings[Pitch::C   ] = MeteorAction::VK24_L_C1 ;
	pitchBindings[Pitch::Db  ] = MeteorAction::VK24_L_bD1;
	pitchBindings[Pitch::D   ] = MeteorAction::VK24_L_D1 ;
	pitchBindings[Pitch::Eb  ] = MeteorAction::VK24_L_bE1;
	pitchBindings[Pitch::E   ] = MeteorAction::VK24_L_E1 ;
	pitchBindings[Pitch::F   ] = MeteorAction::VK24_L_F1 ;
	pitchBindings[Pitch::Gb  ] = MeteorAction::VK24_L_bG1;
	pitchBindings[Pitch::G   ] = MeteorAction::VK24_L_G1 ;
	pitchBindings[Pitch::Ab  ] = MeteorAction::VK24_L_bA1;
	pitchBindings[Pitch::A   ] = MeteorAction::VK24_L_A1 ;
	pitchBindings[Pitch::Bb  ] = MeteorAction::VK24_L_bB1;
	pitchBindings[Pitch::B   ] = MeteorAction::VK24_L_B1 ;
	
	pitchBindings[Pitch::c   ] = MeteorAction::VK24_L_C2 ;
	pitchBindings[Pitch::db  ] = MeteorAction::VK24_L_bD2;
	pitchBindings[Pitch::d   ] = MeteorAction::VK24_L_D2 ;
	pitchBindings[Pitch::eb  ] = MeteorAction::VK24_L_bE2;
	pitchBindings[Pitch::e   ] = MeteorAction::VK24_L_E2 ;
	pitchBindings[Pitch::f   ] = MeteorAction::VK24_L_F2 ;
	pitchBindings[Pitch::gb  ] = MeteorAction::VK24_L_bG2;
	pitchBindings[Pitch::g   ] = MeteorAction::VK24_L_G2 ;
	pitchBindings[Pitch::ab  ] = MeteorAction::VK24_L_bA2;
	pitchBindings[Pitch::a   ] = MeteorAction::VK24_L_A2 ;
	pitchBindings[Pitch::bb  ] = MeteorAction::VK24_L_bB2;
	pitchBindings[Pitch::b   ] = MeteorAction::VK24_L_B2 ;
	
	pitchBindings[Pitch::c1  ] = MeteorAction::VK24_R_C1 ;
	pitchBindings[Pitch::d1b ] = MeteorAction::VK24_R_bD1;
	pitchBindings[Pitch::d1  ] = MeteorAction::VK24_R_D1 ;
	pitchBindings[Pitch::e1b ] = MeteorAction::VK24_R_bE1;
	pitchBindings[Pitch::e1  ] = MeteorAction::VK24_R_E1 ;
	pitchBindings[Pitch::f1  ] = MeteorAction::VK24_R_F1 ;
	pitchBindings[Pitch::g1b ] = MeteorAction::VK24_R_bG1;
	pitchBindings[Pitch::g1  ] = MeteorAction::VK24_R_G1 ;
	pitchBindings[Pitch::a1b ] = MeteorAction::VK24_R_bA1;
	pitchBindings[Pitch::a1  ] = MeteorAction::VK24_R_A1 ;
	pitchBindings[Pitch::b1b ] = MeteorAction::VK24_R_bB1;
	pitchBindings[Pitch::b1  ] = MeteorAction::VK24_R_B1 ;
	
	pitchBindings[Pitch::c2  ] = MeteorAction::VK24_R_C2 ;
	pitchBindings[Pitch::d2b ] = MeteorAction::VK24_R_bD2;
	pitchBindings[Pitch::d2  ] = MeteorAction::VK24_R_D2 ;
	pitchBindings[Pitch::e2b ] = MeteorAction::VK24_R_bE2;
	pitchBindings[Pitch::e2  ] = MeteorAction::VK24_R_E2 ;
	pitchBindings[Pitch::f2  ] = MeteorAction::VK24_R_F2 ;
	pitchBindings[Pitch::g2b ] = MeteorAction::VK24_R_bG2;
	pitchBindings[Pitch::g2  ] = MeteorAction::VK24_R_G2 ;
	pitchBindings[Pitch::a2b ] = MeteorAction::VK24_R_bA2;
	pitchBindings[Pitch::a2  ] = MeteorAction::VK24_R_A2 ;
	pitchBindings[Pitch::b2b ] = MeteorAction::VK24_R_bB2;
	pitchBindings[Pitch::b2  ] = MeteorAction::VK24_R_B2 ;


#pragma endregion

	/* 降八度 */
#pragma region LoweredPitchState

	loweredPitchBindings[Pitch::C1  ] = MeteorAction::VK24_L_C1 ;
	loweredPitchBindings[Pitch::D1b ] = MeteorAction::VK24_L_bD1;
	loweredPitchBindings[Pitch::D1  ] = MeteorAction::VK24_L_D1 ;
	loweredPitchBindings[Pitch::E1b ] = MeteorAction::VK24_L_bE1;
	loweredPitchBindings[Pitch::E1  ] = MeteorAction::VK24_L_E1 ;
	loweredPitchBindings[Pitch::F1  ] = MeteorAction::VK24_L_F1 ;
	loweredPitchBindings[Pitch::G1b ] = MeteorAction::VK24_L_bG1;
	loweredPitchBindings[Pitch::G1  ] = MeteorAction::VK24_L_G1 ;
	loweredPitchBindings[Pitch::A1b ] = MeteorAction::VK24_L_bA1;
	loweredPitchBindings[Pitch::A1  ] = MeteorAction::VK24_L_A1 ;
	loweredPitchBindings[Pitch::B1b ] = MeteorAction::VK24_L_bB1;
	loweredPitchBindings[Pitch::B1  ] = MeteorAction::VK24_L_B1 ;
	
	loweredPitchBindings[Pitch::C   ] = MeteorAction::VK24_L_C2 ;
	loweredPitchBindings[Pitch::Db  ] = MeteorAction::VK24_L_bD2;
	loweredPitchBindings[Pitch::D   ] = MeteorAction::VK24_L_D2 ;
	loweredPitchBindings[Pitch::Eb  ] = MeteorAction::VK24_L_bE2;
	loweredPitchBindings[Pitch::E   ] = MeteorAction::VK24_L_E2 ;
	loweredPitchBindings[Pitch::F   ] = MeteorAction::VK24_L_F2 ;
	loweredPitchBindings[Pitch::Gb  ] = MeteorAction::VK24_L_bG2;
	loweredPitchBindings[Pitch::G   ] = MeteorAction::VK24_L_G2 ;
	loweredPitchBindings[Pitch::Ab  ] = MeteorAction::VK24_L_bA2;
	loweredPitchBindings[Pitch::A   ] = MeteorAction::VK24_L_A2 ;
	loweredPitchBindings[Pitch::Bb  ] = MeteorAction::VK24_L_bB2;
	loweredPitchBindings[Pitch::B   ] = MeteorAction::VK24_L_B2 ;
	
	loweredPitchBindings[Pitch::c   ] = MeteorAction::VK24_R_C1 ;
	loweredPitchBindings[Pitch::db  ] = MeteorAction::VK24_R_bD1;
	loweredPitchBindings[Pitch::d   ] = MeteorAction::VK24_R_D1 ;
	loweredPitchBindings[Pitch::eb  ] = MeteorAction::VK24_R_bE1;
	loweredPitchBindings[Pitch::e   ] = MeteorAction::VK24_R_E1 ;
	loweredPitchBindings[Pitch::f   ] = MeteorAction::VK24_R_F1 ;
	loweredPitchBindings[Pitch::gb  ] = MeteorAction::VK24_R_bG1;
	loweredPitchBindings[Pitch::g   ] = MeteorAction::VK24_R_G1 ;
	loweredPitchBindings[Pitch::ab  ] = MeteorAction::VK24_R_bA1;
	loweredPitchBindings[Pitch::a   ] = MeteorAction::VK24_R_A1 ;
	loweredPitchBindings[Pitch::bb  ] = MeteorAction::VK24_R_bB1;
	loweredPitchBindings[Pitch::b   ] = MeteorAction::VK24_R_B1 ;
	
	loweredPitchBindings[Pitch::c1  ] = MeteorAction::VK24_R_C2 ;
	loweredPitchBindings[Pitch::d1b ] = MeteorAction::VK24_R_bD2;
	loweredPitchBindings[Pitch::d1  ] = MeteorAction::VK24_R_D2 ;
	loweredPitchBindings[Pitch::e1b ] = MeteorAction::VK24_R_bE2;
	loweredPitchBindings[Pitch::e1  ] = MeteorAction::VK24_R_E2 ;
	loweredPitchBindings[Pitch::f1  ] = MeteorAction::VK24_R_F2 ;
	loweredPitchBindings[Pitch::g1b ] = MeteorAction::VK24_R_bG2;
	loweredPitchBindings[Pitch::g1  ] = MeteorAction::VK24_R_G2 ;
	loweredPitchBindings[Pitch::a1b ] = MeteorAction::VK24_R_bA2;
	loweredPitchBindings[Pitch::a1  ] = MeteorAction::VK24_R_A2 ;
	loweredPitchBindings[Pitch::b1b ] = MeteorAction::VK24_R_bB2;
	loweredPitchBindings[Pitch::b1  ] = MeteorAction::VK24_R_B2 ;

#pragma endregion

	/* 升八度 */
#pragma region RaisedPitchState
	 
	raisedPitchBindings[Pitch::c		] =	 MeteorAction::VK24_L_C1 ;
	raisedPitchBindings[Pitch::db		] =	 MeteorAction::VK24_L_bD1;
	raisedPitchBindings[Pitch::d		] =	 MeteorAction::VK24_L_D1 ;
	raisedPitchBindings[Pitch::eb		] =	 MeteorAction::VK24_L_bE1;
	raisedPitchBindings[Pitch::e		] =	 MeteorAction::VK24_L_E1 ;
	raisedPitchBindings[Pitch::f		] =	 MeteorAction::VK24_L_F1 ;
	raisedPitchBindings[Pitch::gb		] =	 MeteorAction::VK24_L_bG1;
	raisedPitchBindings[Pitch::g		] =	 MeteorAction::VK24_L_G1 ;
	raisedPitchBindings[Pitch::ab		] =	 MeteorAction::VK24_L_bA1;
	raisedPitchBindings[Pitch::a		] =	 MeteorAction::VK24_L_A1 ;
	raisedPitchBindings[Pitch::bb		] =	 MeteorAction::VK24_L_bB1;
	raisedPitchBindings[Pitch::b		] =	 MeteorAction::VK24_L_B1 ;

	raisedPitchBindings[Pitch::c1		] =	 MeteorAction::VK24_L_C2 ;
	raisedPitchBindings[Pitch::d1b		] =	 MeteorAction::VK24_L_bD2;
	raisedPitchBindings[Pitch::d1		] =	 MeteorAction::VK24_L_D2 ;
	raisedPitchBindings[Pitch::e1b		] =	 MeteorAction::VK24_L_bE2;
	raisedPitchBindings[Pitch::e1		] =	 MeteorAction::VK24_L_E2 ;
	raisedPitchBindings[Pitch::f1		] =	 MeteorAction::VK24_L_F2 ;
	raisedPitchBindings[Pitch::g1b		] =	 MeteorAction::VK24_L_bG2;
	raisedPitchBindings[Pitch::g1		] =	 MeteorAction::VK24_L_G2 ;
	raisedPitchBindings[Pitch::a1b		] =	 MeteorAction::VK24_L_bA2;
	raisedPitchBindings[Pitch::a1		] =	 MeteorAction::VK24_L_A2 ;
	raisedPitchBindings[Pitch::b1b		] =	 MeteorAction::VK24_L_bB2;
	raisedPitchBindings[Pitch::b1		] =	 MeteorAction::VK24_L_B2 ;

	raisedPitchBindings[Pitch::c2	    ] =	 MeteorAction::VK24_R_C1 ;
	raisedPitchBindings[Pitch::d2b		] =	 MeteorAction::VK24_R_bD1;
	raisedPitchBindings[Pitch::d2	    ] =	 MeteorAction::VK24_R_D1 ;
	raisedPitchBindings[Pitch::e2b		] =	 MeteorAction::VK24_R_bE1;
	raisedPitchBindings[Pitch::e2	    ] =	 MeteorAction::VK24_R_E1 ;
	raisedPitchBindings[Pitch::f2	    ] =	 MeteorAction::VK24_R_F1 ;
	raisedPitchBindings[Pitch::g2b		] =	 MeteorAction::VK24_R_bG1;
	raisedPitchBindings[Pitch::g2	    ] =	 MeteorAction::VK24_R_G1 ;
	raisedPitchBindings[Pitch::a2b		] =	 MeteorAction::VK24_R_bA1;
	raisedPitchBindings[Pitch::a2	    ] =	 MeteorAction::VK24_R_A1 ;
	raisedPitchBindings[Pitch::b2b		] =	 MeteorAction::VK24_R_bB1;
	raisedPitchBindings[Pitch::b2	    ] =	 MeteorAction::VK24_R_B1 ; 

	raisedPitchBindings[Pitch::c3	    ] = MeteorAction::VK24_R_C2 ;
	raisedPitchBindings[Pitch::d3b		] = MeteorAction::VK24_R_bD2;
	raisedPitchBindings[Pitch::d3	    ] = MeteorAction::VK24_R_D2 ;
	raisedPitchBindings[Pitch::e3b		] = MeteorAction::VK24_R_bE2;
	raisedPitchBindings[Pitch::e3	    ] = MeteorAction::VK24_R_E2 ;
	raisedPitchBindings[Pitch::f3	    ] = MeteorAction::VK24_R_F2 ;
	raisedPitchBindings[Pitch::g3b		] = MeteorAction::VK24_R_bG2;
	raisedPitchBindings[Pitch::g3	    ] = MeteorAction::VK24_R_G2 ;
	raisedPitchBindings[Pitch::a3b		] = MeteorAction::VK24_R_bA2;
	raisedPitchBindings[Pitch::a3	    ] = MeteorAction::VK24_R_A2 ;
	raisedPitchBindings[Pitch::b3b		] = MeteorAction::VK24_R_bB2;
	raisedPitchBindings[Pitch::b3	    ] = MeteorAction::VK24_R_B2 ;

#pragma endregion

	return 0;
}

bool MeteorEventProcessorMaster::matchPitch(HitObject * hObject, MeteorAction meteorAction)
{
	HasPitch* hasPitch = dynamic_cast<HasPitch*>(hObject);
	if (hasPitch == nullptr)
		return false;

	switch (pitchState) {

	case MeteoPianoPitchState::None:
		if (pitchBindings.find(hasPitch->GetPitch()) != pitchBindings.end()) {
			if (pitchBindings[hasPitch->GetPitch()] == meteorAction)
				return true;
		}
		break;

	case MeteoPianoPitchState::Lowered:
		if (loweredPitchBindings.find(hasPitch->GetPitch()) != loweredPitchBindings.end()) {
			if (loweredPitchBindings[hasPitch->GetPitch()] == meteorAction)
				return true;
		}
		break;

	case MeteoPianoPitchState::Raised:
		if (raisedPitchBindings.find(hasPitch->GetPitch()) != raisedPitchBindings.end()) {
			if (raisedPitchBindings[hasPitch->GetPitch()] == meteorAction)
				return true;
		}
		break;
	}

	return false;
}

bool MeteorEventProcessorMaster::filterHiddenNoteEffects(EventProcessor<Event>* eventProcessor)
{
	bool isInstrumentEvent = dynamic_cast<InstrumentEvent*>(eventProcessor->GetEvent()) != nullptr ? true : false;

	if (isInstrumentEvent)
		return true;


	Event* sourceEvent = eventProcessor->GetEvent()->GetSourceEvent();

	Event* eventToFilter = sourceEvent == nullptr ? eventProcessor->GetEvent() : sourceEvent;

	if (dynamic_cast<PlayableControlPoint*>(eventToFilter))
	if (dynamic_cast<PlayableControlPoint*>(eventToFilter)->GetHandType() == HandType::Hidden){
		return false;
	}

	return true;
}

Pitch MeteorEventProcessorMaster::GetPitchFromAction(MeteorAction action)
{
	if (action < MeteorAction::VK24_L_C1 || action > MeteorAction::VK24_R_B2)
		return Pitch::None;

	int pitch = (int)action - (int)MeteorAction::VK24_L_C1 + 24;

	switch (pitchState) {
	case MeteoPianoPitchState::Lowered:
		pitch -= 12;
		break;
	case MeteoPianoPitchState::Raised:
		pitch += 12;
		break;
	}

	return (Pitch)pitch;


	// 這樣寫太慢
	map<Pitch, MeteorAction>::iterator it;

	switch (pitchState) {

	case MeteoPianoPitchState::None:
		for (it = pitchBindings.begin(); it != pitchBindings.end(); ++it) {
			if (it->second == action) {
				return it->first;
			}
		}
		break;

	case MeteoPianoPitchState::Lowered:
		for (it = loweredPitchBindings.begin(); it != loweredPitchBindings.end(); ++it) {
			if (it->second == action) {
				return it->first;
			}
		}
		break;

	case MeteoPianoPitchState::Raised:
		for (it = raisedPitchBindings.begin(); it != raisedPitchBindings.end(); ++it) {
			if (it->second == action) {
				return it->first;
			}
		}
		break;
	}

	LOG(LogLevel::Warning) << "EventProcessorMaster::GetPitchFromAction() : pitch not found [" << (int)action << "].";

	return Pitch::None;
}
