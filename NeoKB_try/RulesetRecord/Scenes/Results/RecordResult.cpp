#include "RecordResult.h"

#include "../../../Games/Output/Bluetooths/MeteoContextBluetoothMessage.h"
#include "../../../Games/Output/Bluetooths/MeteoFileBluetoothMessage.h"

using namespace Record::Scenes::Results;
using namespace Games::Output::Bluetooths;


int RecordResult::load()
{
	OutputManager * o = GetCache<OutputManager>("OutputManager");
	if (!o)
		throw runtime_error("MeteorResult::load() : OutputManager not found in cache.");

	return load(o);
}

int RecordResult::load(OutputManager * o)
{
	outputManager = o;
	return 0;
}

RecordResult::RecordResult(Score * s) : Result(s), RegisterType("RecordResult")
{

	registerLoad(bind((int(RecordResult::*)())&RecordResult::load, this));
}

int RecordResult::onEntering(Scene * lastScene)
{
	MeteoScene::onEntering(lastScene);

	LOG(LogLevel::Info) << "InstantResult::onEntering : pushing game over message.";

	/* �٨S�g�n
	// bluetooth���e���G
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
