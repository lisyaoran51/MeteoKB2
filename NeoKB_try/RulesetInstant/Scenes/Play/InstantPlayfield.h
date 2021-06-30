#ifndef INSTANT_PLAYFIELD_H
#define INSTANT_PLAYFIELD_H

#include "../../../Games/Scene/Play/Playfield.h"
#include "../../../Util/MtoType.h"
#include "../../Config/InstantConfigManager.h"
#include "../../../Instruments/Pitch.h"
#include "../../../Framework/Input/KeyBindings/KeyBindingHandler.h"
#include "../../Input/InstantInputManager.h"
#include "../../../Instruments/MeteoPiano.h"
#include "../../Scheduler/Event/InstantEventProcessorMaster.h"
#include "../../../Framework/Input/Messages/MessageHandler.h"



using namespace Util;
using namespace Games::Scenes::Play;
using namespace Games::Schedulers::Events;
using namespace Instant::Config;
using namespace Instruments;
using namespace Instant::Input;
using namespace Framework::Input::KeyBindings;
using namespace Instruments;
using namespace Games::Schedulers::Events::Effects::Algorithms;
using namespace Instant::Schedulers::Events;
using namespace Framework::Input::Messages;



namespace Instant {
namespace Scenes{
namespace Play{

	class InstantPlayfield: public Playfield, public KeyBindingHandler<InstantAction>, public MessageHandler<MeteoBluetoothMessage> {

		int load();

		int load(FrameworkConfigManager* f, InstantConfigManager* m);

	public:

		InstantPlayfield();

		virtual int OnJudgement(HitObject * hitObject, Judgement * judgement);

		int SetWorkingSm(WorkingSm* sm);

		WorkingSm* GetWorkingSm();


		/* KeyBindingHandler<MeteorAction> */

		virtual int OnKeyDown(pair<InstantAction, int> action);

		virtual int OnKeyUp(InstantAction action);

		virtual int OnButtonDown(InstantAction action);

		virtual int OnButtonUp(InstantAction action);

		virtual int OnKnobTurn(pair<InstantAction, int> action);

		virtual int OnSlide(pair<InstantAction, int> action);

	protected:

		InstantEventProcessorMaster* instantEventProcessorMaster = nullptr;

		Pitch startPitch;

		int pitchCount;

		MTO_FLOAT explosionLifeTime = 0.2f;

		WorkingSm* workingSm = nullptr;

		virtual EventProcessorMaster* createEventProcessorMaster();

		virtual DynamicEventGenerator* createDynamicEventGenerator();

		virtual int onMessage(MeteoBluetoothMessage* command);

	};

}}}



#endif