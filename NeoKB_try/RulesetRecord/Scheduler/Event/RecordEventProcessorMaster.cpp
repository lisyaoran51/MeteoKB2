#include "RecordEventProcessorMaster.h"

#include <utility>
#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"



using namespace Record::Schedulers::Events;
using namespace std;
using namespace Games::Output::Bluetooths;




RecordEventProcessorMaster::RecordEventProcessorMaster() : RegisterType("RecordEventProcessorMaster")
{
}

int RecordEventProcessorMaster::ChangePitchState(MeteoPianoPitchState pState)
{
	pitchState = pState;
	return 0;
}

int RecordEventProcessorMaster::OnKeyDown(pair<RecordAction, int> action)
{
	

	return 0;
}

int RecordEventProcessorMaster::OnKeyUp(RecordAction action)
{
	return 0;
}

int RecordEventProcessorMaster::OnButtonDown(RecordAction action)
{
	return 0;
}

int RecordEventProcessorMaster::OnButtonUp(RecordAction action)
{
	return 0;
}

int RecordEventProcessorMaster::OnKnobTurn(pair<RecordAction, int> action)
{
	return 0;
}

int RecordEventProcessorMaster::OnSlide(pair<RecordAction, int> action)
{
	return 0;
}

int RecordEventProcessorMaster::update()
{
	LOG(LogLevel::Depricated) << "RecordEventProcessorMaster::update : update start.";

	EventProcessorMaster::update();

	LOG(LogLevel::Depricated) << "RecordEventProcessorMaster::update : update over.";

	double currentTime = 0;
	/* 這邊要檢查已經過去的fall effect有沒有miss */
	try {
		currentTime = GetClock()->GetCurrentTime();
	}
	catch (exception& e) {
		LOG(LogLevel::Warning) << "RecordEventProcessorMaster::update : clock is not started [" << e.what() << "].";
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

int RecordEventProcessorMaster::LoadOnComplete()
{
	loadAndMapPitches();

	return 0;
}

int RecordEventProcessorMaster::loadAndMapPitches()
{
	/* 未升降 */
#pragma region NonePitchState


	pitchBindings[Pitch::C   ] = RecordAction::VK24_L_C1 ;
	pitchBindings[Pitch::Db  ] = RecordAction::VK24_L_bD1;
	pitchBindings[Pitch::D   ] = RecordAction::VK24_L_D1 ;
	pitchBindings[Pitch::Eb  ] = RecordAction::VK24_L_bE1;
	pitchBindings[Pitch::E   ] = RecordAction::VK24_L_E1 ;
	pitchBindings[Pitch::F   ] = RecordAction::VK24_L_F1 ;
	pitchBindings[Pitch::Gb  ] = RecordAction::VK24_L_bG1;
	pitchBindings[Pitch::G   ] = RecordAction::VK24_L_G1 ;
	pitchBindings[Pitch::Ab  ] = RecordAction::VK24_L_bA1;
	pitchBindings[Pitch::A   ] = RecordAction::VK24_L_A1 ;
	pitchBindings[Pitch::Bb  ] = RecordAction::VK24_L_bB1;
	pitchBindings[Pitch::B   ] = RecordAction::VK24_L_B1 ;
								 
	pitchBindings[Pitch::c   ] = RecordAction::VK24_L_C2 ;
	pitchBindings[Pitch::db  ] = RecordAction::VK24_L_bD2;
	pitchBindings[Pitch::d   ] = RecordAction::VK24_L_D2 ;
	pitchBindings[Pitch::eb  ] = RecordAction::VK24_L_bE2;
	pitchBindings[Pitch::e   ] = RecordAction::VK24_L_E2 ;
	pitchBindings[Pitch::f   ] = RecordAction::VK24_L_F2 ;
	pitchBindings[Pitch::gb  ] = RecordAction::VK24_L_bG2;
	pitchBindings[Pitch::g   ] = RecordAction::VK24_L_G2 ;
	pitchBindings[Pitch::ab  ] = RecordAction::VK24_L_bA2;
	pitchBindings[Pitch::a   ] = RecordAction::VK24_L_A2 ;
	pitchBindings[Pitch::bb  ] = RecordAction::VK24_L_bB2;
	pitchBindings[Pitch::b   ] = RecordAction::VK24_L_B2 ;
								 
	pitchBindings[Pitch::c1  ] = RecordAction::VK24_R_C1 ;
	pitchBindings[Pitch::d1b ] = RecordAction::VK24_R_bD1;
	pitchBindings[Pitch::d1  ] = RecordAction::VK24_R_D1 ;
	pitchBindings[Pitch::e1b ] = RecordAction::VK24_R_bE1;
	pitchBindings[Pitch::e1  ] = RecordAction::VK24_R_E1 ;
	pitchBindings[Pitch::f1  ] = RecordAction::VK24_R_F1 ;
	pitchBindings[Pitch::g1b ] = RecordAction::VK24_R_bG1;
	pitchBindings[Pitch::g1  ] = RecordAction::VK24_R_G1 ;
	pitchBindings[Pitch::a1b ] = RecordAction::VK24_R_bA1;
	pitchBindings[Pitch::a1  ] = RecordAction::VK24_R_A1 ;
	pitchBindings[Pitch::b1b ] = RecordAction::VK24_R_bB1;
	pitchBindings[Pitch::b1  ] = RecordAction::VK24_R_B1 ;
								 
	pitchBindings[Pitch::c2  ] = RecordAction::VK24_R_C2 ;
	pitchBindings[Pitch::d2b ] = RecordAction::VK24_R_bD2;
	pitchBindings[Pitch::d2  ] = RecordAction::VK24_R_D2 ;
	pitchBindings[Pitch::e2b ] = RecordAction::VK24_R_bE2;
	pitchBindings[Pitch::e2  ] = RecordAction::VK24_R_E2 ;
	pitchBindings[Pitch::f2  ] = RecordAction::VK24_R_F2 ;
	pitchBindings[Pitch::g2b ] = RecordAction::VK24_R_bG2;
	pitchBindings[Pitch::g2  ] = RecordAction::VK24_R_G2 ;
	pitchBindings[Pitch::a2b ] = RecordAction::VK24_R_bA2;
	pitchBindings[Pitch::a2  ] = RecordAction::VK24_R_A2 ;
	pitchBindings[Pitch::b2b ] = RecordAction::VK24_R_bB2;
	pitchBindings[Pitch::b2  ] = RecordAction::VK24_R_B2 ;


#pragma endregion

	/* 降八度 */
#pragma region LoweredPitchState

	loweredPitchBindings[Pitch::C1  ] = RecordAction::VK24_L_C1 ;
	loweredPitchBindings[Pitch::D1b ] = RecordAction::VK24_L_bD1;
	loweredPitchBindings[Pitch::D1  ] = RecordAction::VK24_L_D1 ;
	loweredPitchBindings[Pitch::E1b ] = RecordAction::VK24_L_bE1;
	loweredPitchBindings[Pitch::E1  ] = RecordAction::VK24_L_E1 ;
	loweredPitchBindings[Pitch::F1  ] = RecordAction::VK24_L_F1 ;
	loweredPitchBindings[Pitch::G1b ] = RecordAction::VK24_L_bG1;
	loweredPitchBindings[Pitch::G1  ] = RecordAction::VK24_L_G1 ;
	loweredPitchBindings[Pitch::A1b ] = RecordAction::VK24_L_bA1;
	loweredPitchBindings[Pitch::A1  ] = RecordAction::VK24_L_A1 ;
	loweredPitchBindings[Pitch::B1b ] = RecordAction::VK24_L_bB1;
	loweredPitchBindings[Pitch::B1  ] = RecordAction::VK24_L_B1 ;
										
	loweredPitchBindings[Pitch::C   ] = RecordAction::VK24_L_C2 ;
	loweredPitchBindings[Pitch::Db  ] = RecordAction::VK24_L_bD2;
	loweredPitchBindings[Pitch::D   ] = RecordAction::VK24_L_D2 ;
	loweredPitchBindings[Pitch::Eb  ] = RecordAction::VK24_L_bE2;
	loweredPitchBindings[Pitch::E   ] = RecordAction::VK24_L_E2 ;
	loweredPitchBindings[Pitch::F   ] = RecordAction::VK24_L_F2 ;
	loweredPitchBindings[Pitch::Gb  ] = RecordAction::VK24_L_bG2;
	loweredPitchBindings[Pitch::G   ] = RecordAction::VK24_L_G2 ;
	loweredPitchBindings[Pitch::Ab  ] = RecordAction::VK24_L_bA2;
	loweredPitchBindings[Pitch::A   ] = RecordAction::VK24_L_A2 ;
	loweredPitchBindings[Pitch::Bb  ] = RecordAction::VK24_L_bB2;
	loweredPitchBindings[Pitch::B   ] = RecordAction::VK24_L_B2 ;
										
	loweredPitchBindings[Pitch::c   ] = RecordAction::VK24_R_C1 ;
	loweredPitchBindings[Pitch::db  ] = RecordAction::VK24_R_bD1;
	loweredPitchBindings[Pitch::d   ] = RecordAction::VK24_R_D1 ;
	loweredPitchBindings[Pitch::eb  ] = RecordAction::VK24_R_bE1;
	loweredPitchBindings[Pitch::e   ] = RecordAction::VK24_R_E1 ;
	loweredPitchBindings[Pitch::f   ] = RecordAction::VK24_R_F1 ;
	loweredPitchBindings[Pitch::gb  ] = RecordAction::VK24_R_bG1;
	loweredPitchBindings[Pitch::g   ] = RecordAction::VK24_R_G1 ;
	loweredPitchBindings[Pitch::ab  ] = RecordAction::VK24_R_bA1;
	loweredPitchBindings[Pitch::a   ] = RecordAction::VK24_R_A1 ;
	loweredPitchBindings[Pitch::bb  ] = RecordAction::VK24_R_bB1;
	loweredPitchBindings[Pitch::b   ] = RecordAction::VK24_R_B1 ;
										
	loweredPitchBindings[Pitch::c1  ] = RecordAction::VK24_R_C2 ;
	loweredPitchBindings[Pitch::d1b ] = RecordAction::VK24_R_bD2;
	loweredPitchBindings[Pitch::d1  ] = RecordAction::VK24_R_D2 ;
	loweredPitchBindings[Pitch::e1b ] = RecordAction::VK24_R_bE2;
	loweredPitchBindings[Pitch::e1  ] = RecordAction::VK24_R_E2 ;
	loweredPitchBindings[Pitch::f1  ] = RecordAction::VK24_R_F2 ;
	loweredPitchBindings[Pitch::g1b ] = RecordAction::VK24_R_bG2;
	loweredPitchBindings[Pitch::g1  ] = RecordAction::VK24_R_G2 ;
	loweredPitchBindings[Pitch::a1b ] = RecordAction::VK24_R_bA2;
	loweredPitchBindings[Pitch::a1  ] = RecordAction::VK24_R_A2 ;
	loweredPitchBindings[Pitch::b1b ] = RecordAction::VK24_R_bB2;
	loweredPitchBindings[Pitch::b1  ] = RecordAction::VK24_R_B2 ;

#pragma endregion

	/* 升八度 */
#pragma region RaisedPitchState
	 
	raisedPitchBindings[Pitch::c		] =	 RecordAction::VK24_L_C1 ;
	raisedPitchBindings[Pitch::db		] =	 RecordAction::VK24_L_bD1;
	raisedPitchBindings[Pitch::d		] =	 RecordAction::VK24_L_D1 ;
	raisedPitchBindings[Pitch::eb		] =	 RecordAction::VK24_L_bE1;
	raisedPitchBindings[Pitch::e		] =	 RecordAction::VK24_L_E1 ;
	raisedPitchBindings[Pitch::f		] =	 RecordAction::VK24_L_F1 ;
	raisedPitchBindings[Pitch::gb		] =	 RecordAction::VK24_L_bG1;
	raisedPitchBindings[Pitch::g		] =	 RecordAction::VK24_L_G1 ;
	raisedPitchBindings[Pitch::ab		] =	 RecordAction::VK24_L_bA1;
	raisedPitchBindings[Pitch::a		] =	 RecordAction::VK24_L_A1 ;
	raisedPitchBindings[Pitch::bb		] =	 RecordAction::VK24_L_bB1;
	raisedPitchBindings[Pitch::b		] =	 RecordAction::VK24_L_B1 ;
											 
	raisedPitchBindings[Pitch::c1		] =	 RecordAction::VK24_L_C2 ;
	raisedPitchBindings[Pitch::d1b		] =	 RecordAction::VK24_L_bD2;
	raisedPitchBindings[Pitch::d1		] =	 RecordAction::VK24_L_D2 ;
	raisedPitchBindings[Pitch::e1b		] =	 RecordAction::VK24_L_bE2;
	raisedPitchBindings[Pitch::e1		] =	 RecordAction::VK24_L_E2 ;
	raisedPitchBindings[Pitch::f1		] =	 RecordAction::VK24_L_F2 ;
	raisedPitchBindings[Pitch::g1b		] =	 RecordAction::VK24_L_bG2;
	raisedPitchBindings[Pitch::g1		] =	 RecordAction::VK24_L_G2 ;
	raisedPitchBindings[Pitch::a1b		] =	 RecordAction::VK24_L_bA2;
	raisedPitchBindings[Pitch::a1		] =	 RecordAction::VK24_L_A2 ;
	raisedPitchBindings[Pitch::b1b		] =	 RecordAction::VK24_L_bB2;
	raisedPitchBindings[Pitch::b1		] =	 RecordAction::VK24_L_B2 ;
											 
	raisedPitchBindings[Pitch::c2	    ] =	 RecordAction::VK24_R_C1 ;
	raisedPitchBindings[Pitch::d2b		] =	 RecordAction::VK24_R_bD1;
	raisedPitchBindings[Pitch::d2	    ] =	 RecordAction::VK24_R_D1 ;
	raisedPitchBindings[Pitch::e2b		] =	 RecordAction::VK24_R_bE1;
	raisedPitchBindings[Pitch::e2	    ] =	 RecordAction::VK24_R_E1 ;
	raisedPitchBindings[Pitch::f2	    ] =	 RecordAction::VK24_R_F1 ;
	raisedPitchBindings[Pitch::g2b		] =	 RecordAction::VK24_R_bG1;
	raisedPitchBindings[Pitch::g2	    ] =	 RecordAction::VK24_R_G1 ;
	raisedPitchBindings[Pitch::a2b		] =	 RecordAction::VK24_R_bA1;
	raisedPitchBindings[Pitch::a2	    ] =	 RecordAction::VK24_R_A1 ;
	raisedPitchBindings[Pitch::b2b		] =	 RecordAction::VK24_R_bB1;
	raisedPitchBindings[Pitch::b2	    ] =	 RecordAction::VK24_R_B1 ; 
											 
	raisedPitchBindings[Pitch::c3	    ] =  RecordAction::VK24_R_C2 ;
	raisedPitchBindings[Pitch::d3b		] =  RecordAction::VK24_R_bD2;
	raisedPitchBindings[Pitch::d3	    ] =  RecordAction::VK24_R_D2 ;
	raisedPitchBindings[Pitch::e3b		] =  RecordAction::VK24_R_bE2;
	raisedPitchBindings[Pitch::e3	    ] =  RecordAction::VK24_R_E2 ;
	raisedPitchBindings[Pitch::f3	    ] =  RecordAction::VK24_R_F2 ;
	raisedPitchBindings[Pitch::g3b		] =  RecordAction::VK24_R_bG2;
	raisedPitchBindings[Pitch::g3	    ] =  RecordAction::VK24_R_G2 ;
	raisedPitchBindings[Pitch::a3b		] =  RecordAction::VK24_R_bA2;
	raisedPitchBindings[Pitch::a3	    ] =  RecordAction::VK24_R_A2 ;
	raisedPitchBindings[Pitch::b3b		] =  RecordAction::VK24_R_bB2;
	raisedPitchBindings[Pitch::b3	    ] =  RecordAction::VK24_R_B2 ;

#pragma endregion

	return 0;
}

bool RecordEventProcessorMaster::matchPitch(HitObject * hObject, RecordAction instantAction)
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
