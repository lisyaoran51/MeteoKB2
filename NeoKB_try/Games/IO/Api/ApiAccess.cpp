#include "ApiAccess.h"

#include "../../../Framework/Devices/BluetoothDevice.h"

using namespace Games::IO::Api;
using namespace Framework::IO;


ApiAccess::ApiAccess(Host * gHost): RegisterType("ApiAccess")
{

	gameHost = gHost;

}
/*
int ApiAccess::AddOnStartWritingSmFile(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnStartWritingSmFile(callableObject, callback, name);
}

int ApiAccess::AddOnWriteSmFileSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	LOG(LogLevel::Depricated) << "ApiAccess::AddOnWriteSmFileSuccess() : api access setup. ";
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnWriteSmFileSuccess(callableObject, callback, name);
}

int ApiAccess::AddOnStartWritingSoundFilePackage(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnStartWritingSoundFilePackage(callableObject, callback, name);
}

int ApiAccess::AddOnWriteSoundFilePackageSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnWriteSoundFilePackageSuccess(callableObject, callback, name);
}

int ApiAccess::AddOnStartWritingProgram(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnStartWritingProgram(callableObject, callback, name);
}

int ApiAccess::AddOnWriteProgramSuccess(MtoObject * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnWriteProgramSuccess(callableObject, callback, name);
}
*/