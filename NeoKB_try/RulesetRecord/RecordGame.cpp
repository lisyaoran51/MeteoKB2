#include "RecordGame.h"

#include "Config/RecordConfigManager.h"
#include "../Framework/Configurations/FrameworkConfigManager.h"


using namespace Record;
using namespace Record::Config;
using namespace Framework::Configurations;


int RecordGame::load()
{
	
	if (!GetCache<FrameworkConfigManager>("FrameworkConfigManager")) {
		FrameworkConfigManager* fConfigManager = new FrameworkConfigManager();
		fConfigManager->Set(FrameworkSetting::PatternGenerator, "RecordPatternGenerator");
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

RecordGame::RecordGame(): RegisterType("RecordGame")
{
	registerLoad(bind((int(RecordGame::*)())&RecordGame::load, this));
}
