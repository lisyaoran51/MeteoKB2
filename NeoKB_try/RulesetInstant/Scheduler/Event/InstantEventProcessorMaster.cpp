#include "InstantEventProcessorMaster.h"

#include "Effect/InstantFallEffectMapper.h"
#include <utility>
#include "../../../Games/Scheduler/Event/ControlPoints/NoteControlPointHitObject.h"
#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"



using namespace Instant::Schedulers::Events;
using namespace Instant::Schedulers::Events::Effects;
using namespace std;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Games::Output::Bluetooths;




InstantEventProcessorMaster::InstantEventProcessorMaster() : RegisterType("InstantEventProcessorMaster")
{
}

int InstantEventProcessorMaster::ChangePitchState(MeteoPianoPitchState pState)
{
	pitchState = pState;
	return 0;
}

int InstantEventProcessorMaster::OnKeyDown(pair<InstantAction, int> action)
{
	
	MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::InstantPressKey);

	switch (pitchState) {

	case MeteoPianoPitchState::None:
		for (auto it = pitchBindings.begin(); it != pitchBindings.end(); it++)
		{
			if (action.first == (*it).second)
				meteoContextBluetoothMessage->GetContextInJson()["Key"] = int((*it).first);
		}
		break;

	case MeteoPianoPitchState::Lowered:
		for (auto it = loweredPitchBindings.begin(); it != loweredPitchBindings.end(); it++)
		{
			if (action.first == (*it).second)
				meteoContextBluetoothMessage->GetContextInJson()["Key"] = int((*it).first);
		}
		break;

	case MeteoPianoPitchState::Raised:
		for (auto it = raisedPitchBindings.begin(); it != raisedPitchBindings.end(); it++)
		{
			if (action.first == (*it).second)
				meteoContextBluetoothMessage->GetContextInJson()["Key"] = int((*it).first);
		}
		break;

	}

	meteoContextBluetoothMessage->GetContextInJson()["Volume"] = action.second;

	outputManager->PushMessage(meteoContextBluetoothMessage);
	

	return 0;
}

int InstantEventProcessorMaster::OnKeyUp(InstantAction action)
{
	MeteoContextBluetoothMessage* meteoContextBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::InstantReleaseKey);

	switch (pitchState) {

	case MeteoPianoPitchState::None:
		for (auto it = pitchBindings.begin(); it != pitchBindings.end(); it++)
		{
			if (action == (*it).second)
				meteoContextBluetoothMessage->GetContextInJson()["Key"] = int((*it).first);
		}
		break;

	case MeteoPianoPitchState::Lowered:
		for (auto it = loweredPitchBindings.begin(); it != loweredPitchBindings.end(); it++)
		{
			if (action == (*it).second)
				meteoContextBluetoothMessage->GetContextInJson()["Key"] = int((*it).first);
		}
		break;

	case MeteoPianoPitchState::Raised:
		for (auto it = raisedPitchBindings.begin(); it != raisedPitchBindings.end(); it++)
		{
			if (action == (*it).second)
				meteoContextBluetoothMessage->GetContextInJson()["Key"] = int((*it).first);
		}
		break;

	}

	outputManager->PushMessage(meteoContextBluetoothMessage);

	return 0;
}

int InstantEventProcessorMaster::OnButtonDown(InstantAction action)
{
	return 0;
}

int InstantEventProcessorMaster::OnButtonUp(InstantAction action)
{
	return 0;
}

int InstantEventProcessorMaster::OnKnobTurn(pair<InstantAction, int> action)
{
	return 0;
}

int InstantEventProcessorMaster::OnSlide(pair<InstantAction, int> action)
{
	return 0;
}

int InstantEventProcessorMaster::update()
{
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
	/*
	vector<EventProcessor<Event>*> eventProcessors;

	// 拿已經結束的event
	eventProcessorPeriods->GetItemsContainPeriods(make_pair<float, float>(currentTime - visibleTimeRange, (float)currentTime), &eventProcessors);
	LOG(LogLevel::Depricated) << "InstantEventProcessorMaster::update() : filter event processors by [" << eventProcessorFilter << "].";
	eventProcessorFilter->Filter(&eventProcessors);

	for (int i = 0; i < eventProcessors.size(); i++) {
		HitObject* hObject = dynamic_cast<HitObject*>(eventProcessors[i]);

		if (hObject == nullptr)
			continue;

		if (hObject->GetHasJudgementResult())
			continue;

		
		if (hObject->TryJudgement() == -2) {
			// TODO: 目前先不檢查踏板，之後要改成如果有插入踏板就要檢查踏板
			if (dynamic_cast<NoteControlPointHitObject*>(hObject)) {
				hObject->UpdateJudgement(false);
			}
		}
		
	}
	*/
	return 0;
}

int InstantEventProcessorMaster::LoadOnComplete()
{
	loadAndMapPitches();

	return 0;
}

int InstantEventProcessorMaster::loadAndMapPitches()
{
	/* 未升降 */
#pragma region NonePitchState


	pitchBindings[Pitch::C   ] = InstantAction::VK24_L_C1 ;
	pitchBindings[Pitch::Db  ] = InstantAction::VK24_L_bD1;
	pitchBindings[Pitch::D   ] = InstantAction::VK24_L_D1 ;
	pitchBindings[Pitch::Eb  ] = InstantAction::VK24_L_bE1;
	pitchBindings[Pitch::E   ] = InstantAction::VK24_L_E1 ;
	pitchBindings[Pitch::F   ] = InstantAction::VK24_L_F1 ;
	pitchBindings[Pitch::Gb  ] = InstantAction::VK24_L_bG1;
	pitchBindings[Pitch::G   ] = InstantAction::VK24_L_G1 ;
	pitchBindings[Pitch::Ab  ] = InstantAction::VK24_L_bA1;
	pitchBindings[Pitch::A   ] = InstantAction::VK24_L_A1 ;
	pitchBindings[Pitch::Bb  ] = InstantAction::VK24_L_bB1;
	pitchBindings[Pitch::B   ] = InstantAction::VK24_L_B1 ;
	
	pitchBindings[Pitch::c   ] = InstantAction::VK24_L_C2 ;
	pitchBindings[Pitch::db  ] = InstantAction::VK24_L_bD2;
	pitchBindings[Pitch::d   ] = InstantAction::VK24_L_D2 ;
	pitchBindings[Pitch::eb  ] = InstantAction::VK24_L_bE2;
	pitchBindings[Pitch::e   ] = InstantAction::VK24_L_E2 ;
	pitchBindings[Pitch::f   ] = InstantAction::VK24_L_F2 ;
	pitchBindings[Pitch::gb  ] = InstantAction::VK24_L_bG2;
	pitchBindings[Pitch::g   ] = InstantAction::VK24_L_G2 ;
	pitchBindings[Pitch::ab  ] = InstantAction::VK24_L_bA2;
	pitchBindings[Pitch::a   ] = InstantAction::VK24_L_A2 ;
	pitchBindings[Pitch::bb  ] = InstantAction::VK24_L_bB2;
	pitchBindings[Pitch::b   ] = InstantAction::VK24_L_B2 ;
								 
	pitchBindings[Pitch::c1  ] = InstantAction::VK24_R_C1 ;
	pitchBindings[Pitch::d1b ] = InstantAction::VK24_R_bD1;
	pitchBindings[Pitch::d1  ] = InstantAction::VK24_R_D1 ;
	pitchBindings[Pitch::e1b ] = InstantAction::VK24_R_bE1;
	pitchBindings[Pitch::e1  ] = InstantAction::VK24_R_E1 ;
	pitchBindings[Pitch::f1  ] = InstantAction::VK24_R_F1 ;
	pitchBindings[Pitch::g1b ] = InstantAction::VK24_R_bG1;
	pitchBindings[Pitch::g1  ] = InstantAction::VK24_R_G1 ;
	pitchBindings[Pitch::a1b ] = InstantAction::VK24_R_bA1;
	pitchBindings[Pitch::a1  ] = InstantAction::VK24_R_A1 ;
	pitchBindings[Pitch::b1b ] = InstantAction::VK24_R_bB1;
	pitchBindings[Pitch::b1  ] = InstantAction::VK24_R_B1 ;
								 
	pitchBindings[Pitch::c2  ] = InstantAction::VK24_R_C2 ;
	pitchBindings[Pitch::d2b ] = InstantAction::VK24_R_bD2;
	pitchBindings[Pitch::d2  ] = InstantAction::VK24_R_D2 ;
	pitchBindings[Pitch::e2b ] = InstantAction::VK24_R_bE2;
	pitchBindings[Pitch::e2  ] = InstantAction::VK24_R_E2 ;
	pitchBindings[Pitch::f2  ] = InstantAction::VK24_R_F2 ;
	pitchBindings[Pitch::g2b ] = InstantAction::VK24_R_bG2;
	pitchBindings[Pitch::g2  ] = InstantAction::VK24_R_G2 ;
	pitchBindings[Pitch::a2b ] = InstantAction::VK24_R_bA2;
	pitchBindings[Pitch::a2  ] = InstantAction::VK24_R_A2 ;
	pitchBindings[Pitch::b2b ] = InstantAction::VK24_R_bB2;
	pitchBindings[Pitch::b2  ] = InstantAction::VK24_R_B2 ;


#pragma endregion

	/* 降八度 */
#pragma region LoweredPitchState

	loweredPitchBindings[Pitch::C1  ] = InstantAction::VK24_L_C1 ;
	loweredPitchBindings[Pitch::D1b ] = InstantAction::VK24_L_bD1;
	loweredPitchBindings[Pitch::D1  ] = InstantAction::VK24_L_D1 ;
	loweredPitchBindings[Pitch::E1b ] = InstantAction::VK24_L_bE1;
	loweredPitchBindings[Pitch::E1  ] = InstantAction::VK24_L_E1 ;
	loweredPitchBindings[Pitch::F1  ] = InstantAction::VK24_L_F1 ;
	loweredPitchBindings[Pitch::G1b ] = InstantAction::VK24_L_bG1;
	loweredPitchBindings[Pitch::G1  ] = InstantAction::VK24_L_G1 ;
	loweredPitchBindings[Pitch::A1b ] = InstantAction::VK24_L_bA1;
	loweredPitchBindings[Pitch::A1  ] = InstantAction::VK24_L_A1 ;
	loweredPitchBindings[Pitch::B1b ] = InstantAction::VK24_L_bB1;
	loweredPitchBindings[Pitch::B1  ] = InstantAction::VK24_L_B1 ;
										
	loweredPitchBindings[Pitch::C   ] = InstantAction::VK24_L_C2 ;
	loweredPitchBindings[Pitch::Db  ] = InstantAction::VK24_L_bD2;
	loweredPitchBindings[Pitch::D   ] = InstantAction::VK24_L_D2 ;
	loweredPitchBindings[Pitch::Eb  ] = InstantAction::VK24_L_bE2;
	loweredPitchBindings[Pitch::E   ] = InstantAction::VK24_L_E2 ;
	loweredPitchBindings[Pitch::F   ] = InstantAction::VK24_L_F2 ;
	loweredPitchBindings[Pitch::Gb  ] = InstantAction::VK24_L_bG2;
	loweredPitchBindings[Pitch::G   ] = InstantAction::VK24_L_G2 ;
	loweredPitchBindings[Pitch::Ab  ] = InstantAction::VK24_L_bA2;
	loweredPitchBindings[Pitch::A   ] = InstantAction::VK24_L_A2 ;
	loweredPitchBindings[Pitch::Bb  ] = InstantAction::VK24_L_bB2;
	loweredPitchBindings[Pitch::B   ] = InstantAction::VK24_L_B2 ;
										
	loweredPitchBindings[Pitch::c   ] = InstantAction::VK24_R_C1 ;
	loweredPitchBindings[Pitch::db  ] = InstantAction::VK24_R_bD1;
	loweredPitchBindings[Pitch::d   ] = InstantAction::VK24_R_D1 ;
	loweredPitchBindings[Pitch::eb  ] = InstantAction::VK24_R_bE1;
	loweredPitchBindings[Pitch::e   ] = InstantAction::VK24_R_E1 ;
	loweredPitchBindings[Pitch::f   ] = InstantAction::VK24_R_F1 ;
	loweredPitchBindings[Pitch::gb  ] = InstantAction::VK24_R_bG1;
	loweredPitchBindings[Pitch::g   ] = InstantAction::VK24_R_G1 ;
	loweredPitchBindings[Pitch::ab  ] = InstantAction::VK24_R_bA1;
	loweredPitchBindings[Pitch::a   ] = InstantAction::VK24_R_A1 ;
	loweredPitchBindings[Pitch::bb  ] = InstantAction::VK24_R_bB1;
	loweredPitchBindings[Pitch::b   ] = InstantAction::VK24_R_B1 ;
										
	loweredPitchBindings[Pitch::c1  ] = InstantAction::VK24_R_C2 ;
	loweredPitchBindings[Pitch::d1b ] = InstantAction::VK24_R_bD2;
	loweredPitchBindings[Pitch::d1  ] = InstantAction::VK24_R_D2 ;
	loweredPitchBindings[Pitch::e1b ] = InstantAction::VK24_R_bE2;
	loweredPitchBindings[Pitch::e1  ] = InstantAction::VK24_R_E2 ;
	loweredPitchBindings[Pitch::f1  ] = InstantAction::VK24_R_F2 ;
	loweredPitchBindings[Pitch::g1b ] = InstantAction::VK24_R_bG2;
	loweredPitchBindings[Pitch::g1  ] = InstantAction::VK24_R_G2 ;
	loweredPitchBindings[Pitch::a1b ] = InstantAction::VK24_R_bA2;
	loweredPitchBindings[Pitch::a1  ] = InstantAction::VK24_R_A2 ;
	loweredPitchBindings[Pitch::b1b ] = InstantAction::VK24_R_bB2;
	loweredPitchBindings[Pitch::b1  ] = InstantAction::VK24_R_B2 ;

#pragma endregion

	/* 升八度 */
#pragma region RaisedPitchState
	 
	raisedPitchBindings[Pitch::c		] =	 InstantAction::VK24_L_C1 ;
	raisedPitchBindings[Pitch::db		] =	 InstantAction::VK24_L_bD1;
	raisedPitchBindings[Pitch::d		] =	 InstantAction::VK24_L_D1 ;
	raisedPitchBindings[Pitch::eb		] =	 InstantAction::VK24_L_bE1;
	raisedPitchBindings[Pitch::e		] =	 InstantAction::VK24_L_E1 ;
	raisedPitchBindings[Pitch::f		] =	 InstantAction::VK24_L_F1 ;
	raisedPitchBindings[Pitch::gb		] =	 InstantAction::VK24_L_bG1;
	raisedPitchBindings[Pitch::g		] =	 InstantAction::VK24_L_G1 ;
	raisedPitchBindings[Pitch::ab		] =	 InstantAction::VK24_L_bA1;
	raisedPitchBindings[Pitch::a		] =	 InstantAction::VK24_L_A1 ;
	raisedPitchBindings[Pitch::bb		] =	 InstantAction::VK24_L_bB1;
	raisedPitchBindings[Pitch::b		] =	 InstantAction::VK24_L_B1 ;
											 
	raisedPitchBindings[Pitch::c1		] =	 InstantAction::VK24_L_C2 ;
	raisedPitchBindings[Pitch::d1b		] =	 InstantAction::VK24_L_bD2;
	raisedPitchBindings[Pitch::d1		] =	 InstantAction::VK24_L_D2 ;
	raisedPitchBindings[Pitch::e1b		] =	 InstantAction::VK24_L_bE2;
	raisedPitchBindings[Pitch::e1		] =	 InstantAction::VK24_L_E2 ;
	raisedPitchBindings[Pitch::f1		] =	 InstantAction::VK24_L_F2 ;
	raisedPitchBindings[Pitch::g1b		] =	 InstantAction::VK24_L_bG2;
	raisedPitchBindings[Pitch::g1		] =	 InstantAction::VK24_L_G2 ;
	raisedPitchBindings[Pitch::a1b		] =	 InstantAction::VK24_L_bA2;
	raisedPitchBindings[Pitch::a1		] =	 InstantAction::VK24_L_A2 ;
	raisedPitchBindings[Pitch::b1b		] =	 InstantAction::VK24_L_bB2;
	raisedPitchBindings[Pitch::b1		] =	 InstantAction::VK24_L_B2 ;
											 
	raisedPitchBindings[Pitch::c2	    ] =	 InstantAction::VK24_R_C1 ;
	raisedPitchBindings[Pitch::d2b		] =	 InstantAction::VK24_R_bD1;
	raisedPitchBindings[Pitch::d2	    ] =	 InstantAction::VK24_R_D1 ;
	raisedPitchBindings[Pitch::e2b		] =	 InstantAction::VK24_R_bE1;
	raisedPitchBindings[Pitch::e2	    ] =	 InstantAction::VK24_R_E1 ;
	raisedPitchBindings[Pitch::f2	    ] =	 InstantAction::VK24_R_F1 ;
	raisedPitchBindings[Pitch::g2b		] =	 InstantAction::VK24_R_bG1;
	raisedPitchBindings[Pitch::g2	    ] =	 InstantAction::VK24_R_G1 ;
	raisedPitchBindings[Pitch::a2b		] =	 InstantAction::VK24_R_bA1;
	raisedPitchBindings[Pitch::a2	    ] =	 InstantAction::VK24_R_A1 ;
	raisedPitchBindings[Pitch::b2b		] =	 InstantAction::VK24_R_bB1;
	raisedPitchBindings[Pitch::b2	    ] =	 InstantAction::VK24_R_B1 ; 
											 
	raisedPitchBindings[Pitch::c3	    ] =  InstantAction::VK24_R_C2 ;
	raisedPitchBindings[Pitch::d3b		] =  InstantAction::VK24_R_bD2;
	raisedPitchBindings[Pitch::d3	    ] =  InstantAction::VK24_R_D2 ;
	raisedPitchBindings[Pitch::e3b		] =  InstantAction::VK24_R_bE2;
	raisedPitchBindings[Pitch::e3	    ] =  InstantAction::VK24_R_E2 ;
	raisedPitchBindings[Pitch::f3	    ] =  InstantAction::VK24_R_F2 ;
	raisedPitchBindings[Pitch::g3b		] =  InstantAction::VK24_R_bG2;
	raisedPitchBindings[Pitch::g3	    ] =  InstantAction::VK24_R_G2 ;
	raisedPitchBindings[Pitch::a3b		] =  InstantAction::VK24_R_bA2;
	raisedPitchBindings[Pitch::a3	    ] =  InstantAction::VK24_R_A2 ;
	raisedPitchBindings[Pitch::b3b		] =  InstantAction::VK24_R_bB2;
	raisedPitchBindings[Pitch::b3	    ] =  InstantAction::VK24_R_B2 ;

#pragma endregion

	return 0;
}

bool InstantEventProcessorMaster::matchPitch(HitObject * hObject, InstantAction instantAction)
{
	HasPitch* hasPitch = dynamic_cast<HasPitch*>(hObject);
	if (hasPitch == nullptr)
		return false;

	switch (pitchState) {

	case MeteoPianoPitchState::None:
		if (pitchBindings.find(hasPitch->GetPitch()) != pitchBindings.end()) {
			if (pitchBindings[hasPitch->GetPitch()] == instantAction)
				return true;
		}
		break;

	case MeteoPianoPitchState::Lowered:
		if (loweredPitchBindings.find(hasPitch->GetPitch()) != loweredPitchBindings.end()) {
			if (loweredPitchBindings[hasPitch->GetPitch()] == instantAction)
				return true;
		}
		break;

	case MeteoPianoPitchState::Raised:
		if (raisedPitchBindings.find(hasPitch->GetPitch()) != raisedPitchBindings.end()) {
			if (raisedPitchBindings[hasPitch->GetPitch()] == instantAction)
				return true;
		}
		break;
	}

	return false;
}
