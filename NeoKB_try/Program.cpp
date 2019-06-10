#include "Program.h"

#include "Base/Play/Session.h"
#include "Util/ProgramInitializer.h"
#include <thread>

using namespace Base;
using namespace Meteor;
using namespace Base::Play;
using namespace Util;
using namespace Util::Hierachal;


string aplayCommand = ""; // �o�椧��n�R��

void Aplay(){
	
	/*
	system("aplay Resources/Sms/say_seperate.wav");
	*/
	
	system(aplayCommand.c_str());
}

int main(int argc, char *argv[]) {

	LOG(LogLevel::Info) << "int main(int,char*) : Program start. " << argc;

	// �إ�instance creator�Msm decoder
	ProgramInitializer::Initialize();

	/* �Ȯɥ�argv�ӳ]�wlog level */
	Util::logLevel = (LogLevel)atoi(argv[2]);

	LOG(LogLevel::Info) << "int main(int,char*) : Create game [MeteorGame].";
	Game* game = new MeteorGame();


	LOG(LogLevel::Fine) << "int main(int,char*) : Create Config Manager [FrameworkConfigManager].";
	FrameworkConfigManager* fConfigManager = new FrameworkConfigManager();
	fConfigManager->Async();
	/**********/
	aplayCommand = string("aplay Resources/Sms/") + argv[1] + string(".wav");			// �o�椧��n�R��
	fConfigManager->Set(FrameworkSetting::SongTitle				, string(argv[1]));  // �o�椧��n�R��
	/**********/
	fConfigManager->Set(FrameworkSetting::PatternGenerator		, string("MeteorPatternGenerator"));
	fConfigManager->Set(FrameworkSetting::HardwareVersion		, 10);
	fConfigManager->Set(FrameworkSetting::Width					, 48); //�n��
	fConfigManager->Set(FrameworkSetting::Height				, 24);
	fConfigManager->Set(FrameworkSetting::BlackKeyHeight		, 16);	
	fConfigManager->Set(FrameworkSetting::TargetHeight			, 15);	// �C20	// ��15
	fConfigManager->Set(FrameworkSetting::BlackKeyTargetHeight	, 16);	// 14
	fConfigManager->Set(FrameworkSetting::StartPitch			, 24);
	fConfigManager->Set(FrameworkSetting::FrameRate				, 30);
	game->Cache<FrameworkConfigManager>(fConfigManager);


	LOG(LogLevel::Fine) << "int main(int,char*) : Create Config Manager [MeteorConfigManager].";
	MeteorConfigManager* mConfigManager = new MeteorConfigManager();
	mConfigManager->Async();
	mConfigManager->Set(MeteorSetting::WhiteKeyTargetHeight	, 9);	// �C4	  ��9
	mConfigManager->Set(MeteorSetting::BlackKeyTargetHeight	, 8);
	mConfigManager->Set(MeteorSetting::FallSpeed			, MTO_FLOAT(8.f));	//���`16.f	�C12.f
	mConfigManager->Set(MeteorSetting::FallBrightness		, MTO_FLOAT(0.6f));
	mConfigManager->Set(MeteorSetting::FallLength			, 1);
	mConfigManager->Set(MeteorSetting::ExplodeSpeed			, MTO_FLOAT(0.6f));
	mConfigManager->Set(MeteorSetting::ExplodeBrightness	, MTO_FLOAT(0.6f));
	mConfigManager->Set(MeteorSetting::GlowLineSpeed		, MTO_FLOAT(0.6f));
	mConfigManager->Set(MeteorSetting::GlowLineDuration		, MTO_FLOAT(0.6f));
	mConfigManager->Set(MeteorSetting::GlowLineBrightness	, MTO_FLOAT(0.6f));
	mConfigManager->Set(MeteorSetting::TargetLineBlinkSpeed	, MTO_FLOAT(0.75f));
	mConfigManager->Set(MeteorSetting::TargetLineBrightness	, MTO_FLOAT(0.05f));
	if(argc > 3)
	mConfigManager->Set(MeteorSetting::RestartSection		, atoi(argv[3]));
	game->Cache<MeteorConfigManager>(mConfigManager);


	// 1. cache session
	// TODO: �����session�令�ڭn��

	LOG(LogLevel::Info) << "int main(int,char*) : Create Sm Manager.";
	SmManager* smManager = new SmManager();

	LOG(LogLevel::Info) << "int main(int,char*) : Importing Sheetmusics.";
	smManager->Import(new vector<string>{ "Resources/Sms" });

	LOG(LogLevel::Info) << "int main(int,char*) : Create Session with Meteor ruleset and Sm manager.";
	Session* session = new Session(new RulesetInfo("MeteorRuleset"), smManager);

	game->Cache<Session>(session);

	game->Async();

	LOG(LogLevel::Info) << "int main(int,char*) : Run the game.";
	
	thread t(Aplay);
	t.detach();
	
	game->Run();


	return 0;
}
