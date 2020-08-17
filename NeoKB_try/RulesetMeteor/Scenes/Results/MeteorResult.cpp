#include "MeteorResult.h"

#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Games/Output/Bluetooths/MeteoFileBluetoothMessage.h"

using namespace Meteor::Scenes::Results;
using namespace Games::Output::Bluetooths;


int MeteorResult::load()
{
	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("MeteorResult::load() : OutputManager not found in cache.");

	return load(o);
}

int MeteorResult::load(OutputManager * o)
{
	outputManager = o;
	return 0;
}

MeteorResult::MeteorResult(Score * s) : Result(s), RegisterType("MeteorResult")
{

	registerLoad(bind((int(MeteorResult::*)())&MeteorResult::load, this));
}

int MeteorResult::onEntering(Scene * lastScene)
{
	LOG(LogLevel::Info) << "MeteorResult::onEntering : pushing game over message.";

	/* 還沒寫好
	// bluetooth推送結果
	MeteoBluetoothMessage* meteoBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::FinalScore);
	outputManager->PushMessage(meteoBluetoothMessage);

	meteoBluetoothMessage = new MeteoContextBluetoothMessage(MeteoCommand::PlayRecordData);
	outputManager->PushMessage(meteoBluetoothMessage);

	string recordFilePath;

	meteoBluetoothMessage = new MeteoFileBluetoothMessage(MeteoCommand::PlayRecordFileSegment, recordFilePath);
	outputManager->PushMessage(meteoBluetoothMessage);
	*/

	Exit();

	return 0;
}
