#include "GameHost.h"

#include "../../Desktop/Devices/MeteoBluetoothDevice.h"
#include "../../Desktop/Devices/MeteoBitbangDisplayDevice.h"
#include "../../Desktop/Devices/MeteoKeyboardDevice.h"
#include "../../Desktop/Devices/MeteoPanelDevice.h"
#include "../../Framework/Input/Handler/BluetoothInputHandler.h"
#include "../../Framework/Input/Handler/KeyboardInputHandler.h"
#include "../../Framework/Input/Handler/PanelInputHandler.h"
#include "../../Framework/Output/OutputManager.h"
#include "../../RulesetMeteor/Output/Panels/SustainPedalLightRingOutputer.h"
#include "../Output/Panels/RevolveLightRingOutputer.h"
#include "../Output/Panels/FadeLightRingOutputer.h"
#include "../Output/Panels/SpeedRingOutputer.h"
#include "../Output/Panels/IndicatorLightOutputer.h"
#include "../../RulesetMeteor/Config/MeteorConfigManager.h"
#include "../../RulesetInstant/Config/InstantConfigManager.h"



using namespace Games::Hosts;
using namespace Framework::Hosts;
using namespace Framework::Devices;
using namespace Framework::Input::Handler;
using namespace Desktop::Devices;
using namespace Framework::IO;
using namespace Framework::Output;
using namespace Meteor::Output::Panels;
using namespace Games::Output::Panels;
using namespace Meteor::Config;
using namespace Instant::Config;



GameHost::GameHost() : RegisterType("GameHost")
{
}

int GameHost::SetupOutputManager(OutputManager * oManager)
{
	// �o��o�A�줣�Pruleset���S�ġA�Pı���Ӧ۰ʥͦ��A�M��۰ʥh��U��ruleset�̭����S�ĳB�z���~��

	OutputComponent* sustainPedalLightRingOutputer = new SustainPedalLightRingOutputer();
	sustainPedalLightRingOutputer->SetupPeripheral(mainInterface);
	oManager->AddItem(sustainPedalLightRingOutputer);

	OutputComponent* revolveLightRingOutputer = new RevolveLightRingOutputer();
	revolveLightRingOutputer->SetupPeripheral(mainInterface);
	oManager->AddItem(revolveLightRingOutputer);

	OutputComponent* fadeLightRingOutputer = new FadeLightRingOutputer();
	fadeLightRingOutputer->SetupPeripheral(mainInterface);
	oManager->AddItem(fadeLightRingOutputer);

	OutputComponent* speedRingOutputer = new SpeedRingOutputer();
	speedRingOutputer->SetupPeripheral(mainInterface);
	oManager->AddItem(speedRingOutputer);

	OutputComponent* indicatorLightOutputer = new IndicatorLightOutputer();
	indicatorLightOutputer->SetupPeripheral(mainInterface);
	oManager->AddItem(indicatorLightOutputer);

	return 0;
}

vector<InputHandler*>* GameHost::createAvailableInputHandlers()
{
	LOG(LogLevel::Info) << "GameHost::createAvailableInputHandlers() : create available input handlers.";

	// �o�������٬O�n�\�bframework�̭�����X�z

	vector<InputHandler*>* handlers = new vector<InputHandler*>();
	handlers->push_back(new KeyboardInputHandler());
	handlers->push_back(new PanelInputHandler());
	handlers->push_back(new BluetoothInputHandler());

	return handlers;
}

int GameHost::setupConfig()
{
	// �o��Ψ줣�Pruleset��config�A���ӭn�Φ۰ʥͦ�

	Host::setupConfig();

	MeteorConfigManager* meteorConfigManager = new MeteorConfigManager();
	meteorConfigManager->Initialize(storage);

	meteorConfigManager->Set(MeteorSetting::WhiteKeyTargetHeight, 15);	// �C4	  ��9 // �C15	// ��10
	meteorConfigManager->Set(MeteorSetting::BlackKeyTargetHeight, 10);	//10
	meteorConfigManager->Set(MeteorSetting::FallSpeed, MTO_FLOAT(16.f));	//���`16.f	�C12.f �ثe24 unravel36
	meteorConfigManager->Set(MeteorSetting::FallBrightness, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::FallLength, 1);
	meteorConfigManager->Set(MeteorSetting::ExplodeSpeed, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::ExplodeBrightness, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::ExplodeLifeTime, MTO_FLOAT(0.1f));
	meteorConfigManager->Set(MeteorSetting::GlowLineSpeed, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::GlowLineDuration, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::GlowLineBrightness, MTO_FLOAT(0.6f));
	meteorConfigManager->Set(MeteorSetting::TargetLineBlinkSpeed, MTO_FLOAT(0.75f));
	meteorConfigManager->Set(MeteorSetting::TargetLineBrightness, MTO_FLOAT(0.05f));
	dependencies->Cache<MeteorConfigManager>(meteorConfigManager);


	InstantConfigManager* instantConfigManager = new InstantConfigManager();
	instantConfigManager->Initialize(storage);
	dependencies->Cache<InstantConfigManager>(instantConfigManager);

	return 0;
}
