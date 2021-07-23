#include"InstantGame.h"

#include "Config/InstantConfigManager.h"
#include "../Framework/Configurations/FrameworkConfigManager.h"


using namespace Instant;
using namespace Instant::Config;
using namespace Framework::Configurations;


int InstantGame::load()
{
	
	if (!GetCache<FrameworkConfigManager>("FrameworkConfigManager")) {
		FrameworkConfigManager* fConfigManager = new FrameworkConfigManager();
		fConfigManager->Set(FrameworkSetting::PatternGenerator, "InstantPatternGenerator");
		//fConfigManager->Set(FrameworkSetting::HardwareVersion, 10);
		fConfigManager->Set(FrameworkSetting::Width, 12); //�n��
		fConfigManager->Set(FrameworkSetting::Height, 24);
		fConfigManager->Set(FrameworkSetting::BlackKeyHeight, 16);
		fConfigManager->Set(FrameworkSetting::TargetHeight, 20);
		fConfigManager->Set(FrameworkSetting::BlackKeyTargetHeight, 14);
		fConfigManager->Set(FrameworkSetting::StartPitch, 48);
		// TODO: �]�w....
		Cache<FrameworkConfigManager>(fConfigManager);
	}


	

	return 0;
}

InstantGame::InstantGame(): RegisterType("InstantGame")
{
	registerLoad(bind((int(InstantGame::*)())&InstantGame::load, this));
}
