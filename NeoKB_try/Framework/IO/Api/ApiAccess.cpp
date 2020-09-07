#include "ApiAccess.h"

#include "../BluetoothPhone.h"

using namespace Framework::IO::Api;
using namespace Framework::IO;


ApiAccess::ApiAccess(GameHost * gHost): RegisterType("ApiAccess")
{

	gameHost = gHost;

}

template<class _Type>
int ApiAccess::AddOnStartWritingSmFile(_Type * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnStartWritingSmFile(callableObject, callback, name);
}

template<class _Type>
int ApiAccess::AddOnWriteSmFileSuccess(_Type * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnWriteSmFileSuccess(callableObject, callback, name);
}

template<class _Type>
int ApiAccess::AddOnStartWritingSoundFilePackage(_Type * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnStartWritingSoundFilePackage(callableObject, callback, name);
}

template<class _Type>
int ApiAccess::AddOnWriteSoundFilePackageSuccess(_Type * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnWriteSoundFilePackageSuccess(callableObject, callback, name);
}

template<class _Type>
int ApiAccess::AddOnStartWritingProgram(_Type * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnStartWritingProgram(callableObject, callback, name);
}

template<class _Type>
int ApiAccess::AddOnWriteProgramSuccess(_Type * callableObject, function<int(string)> callback, string name)
{
	return gameHost->GetMainInterface()->GetBluetoothPhone()->AddOnWriteProgramSuccess(callableObject, callback, name);
}