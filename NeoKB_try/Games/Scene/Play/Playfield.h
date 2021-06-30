#ifndef PLAYFIELD_H
#define PLAYFIELD_H


#include "../../Scheduler/Event/EventProcessorMaster.h"
#include "../../Scheduler/Event/DynamicEventGenerator.h"
#include "../../Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../../Framework/Configurations/FrameworkConfigManager.h"
#include "../../../Framework/Allocation/Hierachal/Container.h"
#include "../../Scheduler/Event/HitObject.h"
#include "../../../Instruments/MeteoPianoPitchState.h"
#include "../../Scheduler/Event/IoEvents/IoCommunicators/IoCommunicator.h"
#include "../../Scheduler/Event/InstrumentEvents/InstrumentControllers/InstrumentController.h"
#include "../../Scheduler/Event/PlayfieldEvents/PlayfieldControllers/PlayfieldController.h"
#include "../../Scheduler/Event/TimeEvents/TimeControllerControllers/TimeControllerController.h"
#include "../../Scheduler/Event/SystemEvents/SystemControllers/SystemController.h"
#include "../../Scheduler/Event/RecorderEvents/RecorderControllers/RecorderController.h"



using namespace Games::Schedulers::Events;
using namespace Games::Schedulers;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Framework::Configurations;
using namespace Framework::Allocation::Hierachal;
using namespace Instruments;
using namespace Games::Schedulers::Events::IoEvents::IoCommunicators;
using namespace Games::Schedulers::Events::InstrumentEvents::InstrumentControllers;
using namespace Games::Schedulers::Events::PlayfieldEvents::PlayfieldControllers;
using namespace Games::Schedulers::Events::TimeEvents::TimeControllerControllers;
using namespace Games::Schedulers::Events::SystemEvents::SystemControllers;
using namespace Games::Schedulers::Events::RecorderEvents::RecorderControllers;




namespace Games {
namespace Scenes {
namespace Play {

	/// <summary>
	/// when playfield start, it should connect Scheduler, master, renderer with Updater.
	/// So that these objects could be updated on each frame.
	/// should they be connected in this class????
	/// ���o�Ӥ��Oscene�A���ӭn����UI�h	
	/// </summary>
	class Playfield: public Container {


		int width, height;

		int load();

		// �~��playfield��class�A�bload���ɭԤ@�w�n�gŪ��map algo���u�@
		int load(FrameworkConfigManager* f, OutputManager* o);


	public:

		Playfield();
		virtual ~Playfield();

		virtual int LoadOnComplete();

		virtual int OnJudgement(HitObject* hitObject, Judgement* judgement);

		/// <summary>
		/// add events to EventProcessorMaster
		/// </summary>
		virtual int Add(EventProcessor<Event>* ep);

		int SetGetEventProcessorFunction(function<EventProcessor<Event>*(Event*)> getEventProcessorFunction);

		int SetLeaveGameFunction(function<int()> lGame);

		int SetRestartGameFunction(function<int()> rGame);

		int SetEndGameFunction(function<int()> eGame);

		vector<EventProcessor<Event>*>* GetEventProcessors();

		/// <summary>
		/// add dynamic events to EventProcessorMaster
		/// </summary>
		virtual int AddDynamic(EventProcessor<Event>* ep);

		virtual int AddDynamic(Event* e);

		int GetWidth();

		int GetHeight();

		EventProcessorMaster* GetEventProcessorMaster();

		DynamicEventGenerator* GetDynamicEventGenerator();

		/* -----------------Pitch State----------------- */

		int SetIsGameControllingPitchState(bool value);

		int ChangePitchState(MeteoPianoPitchState s);

		MeteoPianoPitchState GetMeteoPianoPitchState();

		int GetXPositionFromPitch(Pitch p);

	protected:

		OutputManager* outputManager = nullptr;

		function<EventProcessor<Event>*(Event*)> getEventProcessor;


		///<summary>
		/// �b�o��s�F�|�Ψ쪺algo�A�bgetEventProcessor�ɥi�H��o���ܭn�Ϊ�algo
		///</summary>
		map<string, SystemController*> systemControllers;

		///<summary>
		/// �b�o��s�F�|�Ψ쪺algo�A�bgetEventProcessor�ɥi�H��o���ܭn�Ϊ�algo
		///</summary>
		map<string, MapAlgorithmInterface*> mapAlgorithms;


		///<summary>
		/// �b�o��s�F�|�Ψ쪺io communicator�A�bgetEventProcessor�ɥi�H��o���ܭn�Ϊ�communicator
		///</summary>
		map<string, IoCommunicatorInterface*> ioCommunicators;

		///<summary>
		/// �b�o��s�F�|�Ψ쪺instrument controller�A�bgetEventProcessor�ɥi�H��o���ܭn�Ϊ�controller
		///</summary>
		map<string, InstrumentControllerInterface*> instrumentControllers;

		///<summary>
		/// �b�o��s�F�|�Ψ쪺playfield controller�A�bgetEventProcessor�ɥi�H��o���ܭn�Ϊ�controller
		///</summary>
		map<string, PlayfieldControllerInterface*> playfieldControllers;

		///<summary>
		/// �b�o��s�F�|�Ψ쪺time controller controller�A�bgetEventProcessor�ɥi�H��o���ܭn�Ϊ�controller
		///</summary>
		map<string, TimeControllerControllerInterface*> timeControllerControllers;

		///<summary>
		/// �b�o��s�F�|�Ψ쪺time controller controller�A�bgetEventProcessor�ɥi�H��o���ܭn�Ϊ�controller
		///</summary>
		map<string, RecorderControllerInterface*> recordControllers;



		vector<EventProcessor<Event>*> eventProcessors;

		EventProcessorMaster* eventProcessorMaster = nullptr;

		DynamicEventGenerator* dynamicEventGenerator = nullptr;

		///<summary>
		/// ���s��ا����v���A�٨S�첾�쥿�T��m�W
		///</summary>
		Map* bufferMap = nullptr;

		function<int()> leaveGame;

		function<int()> restartGame;

		function<int()> endGame;

		Pitch startPitch = Pitch::C1;

		int pitchCount = 48;

		bool isGameControllingPitchState = true;

		MeteoPianoPitchState pitchState = MeteoPianoPitchState::None;

		virtual EventProcessorMaster* createEventProcessorMaster() = 0;

		virtual DynamicEventGenerator* createDynamicEventGenerator() = 0;

	};


}}}











#endif