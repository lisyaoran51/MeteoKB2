#include "JudgementFlasher.h"


using namespace Games::Scenes::Play::Hud;



int JudgementFlasher::load()
{
	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("HudDisplay::load() : OutputManager not found in cache.");

	outputManager = o;

	return 0;
}


JudgementFlasher::JudgementFlasher(): RegisterType("JudgementFlasher")
{
	registerLoad(bind((int(JudgementFlasher::*)())&JudgementFlasher::load, this));
}

int JudgementFlasher::FlashJudgement(Judgement * judgement)
{
	//outputManager...

	LOG(LogLevel::Info) << "JudgementFlasher::FlashJudgement : Flashing Judgement [" << judgement->GetResultString() << "].";

	return 0;
}
