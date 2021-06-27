#include "JudgementFlasher.h"

#include "../../../Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../../Framework/Output/OutputManager.h"
#include <iomanip>
#include <sstream>



using namespace Games::Scenes::Play::Hud;
using namespace Games::Output::Bluetooths;
using namespace Framework::Output;



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

	LOG(LogLevel::Fine) << "JudgementFlasher::FlashJudgement : Flashing Judgement [" << judgement->GetResultString() << "].";




	return 0;
}
