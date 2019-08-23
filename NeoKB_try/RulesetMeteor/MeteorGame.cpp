#include"MeteorGame.h"

#include "Config/MeteorConfigManager.h"
#include "../Framework/Configurations/FrameworkConfigManager.h"


using namespace Meteor;
using namespace Meteor::Config;
using namespace Framework::Configurations;


int MeteorGame::load()
{
	
	if (!GetCache<FrameworkConfigManager>("FrameworkConfigManager")) {
		FrameworkConfigManager* fConfigManager = new FrameworkConfigManager();
		fConfigManager->Set(FrameworkSetting::PatternGenerator, "MeteorPatternGenerator");
		fConfigManager->Set(FrameworkSetting::HardwareVersion, 10);
		fConfigManager->Set(FrameworkSetting::Width, 12); //要改
		fConfigManager->Set(FrameworkSetting::Height, 24);
		fConfigManager->Set(FrameworkSetting::BlackKeyHeight, 16);
		fConfigManager->Set(FrameworkSetting::TargetHeight, 20);
		fConfigManager->Set(FrameworkSetting::BlackKeyTargetHeight, 14);
		fConfigManager->Set(FrameworkSetting::StartPitch, 48);
		// TODO: 設定....
		Cache<FrameworkConfigManager>(fConfigManager);
	}


	

	return 0;
}

MeteorGame::MeteorGame(): RegisterType("MeteorGame")
{
	registerLoad(bind((int(MeteorGame::*)())&MeteorGame::load, this));
}
