#include "InstantResult.h"

#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Games/Output/Bluetooths/MeteoFileBluetoothMessage.h"

using namespace Instant::Scenes::Results;
using namespace Games::Output::Bluetooths;


int InstantResult::load()
{
	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("MeteorResult::load() : OutputManager not found in cache.");

	return load(o);
}

int InstantResult::load(OutputManager * o)
{
	outputManager = o;
	return 0;
}

InstantResult::InstantResult(Score * s) : Result(s), RegisterType("InstantResult")
{

	registerLoad(bind((int(InstantResult::*)())&InstantResult::load, this));
}

int InstantResult::onEntering(Scene * lastScene)
{
	MeteoScene::onEntering(lastScene);

	LOG(LogLevel::Info) << "InstantResult::onEntering : pushing game over message.";

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
