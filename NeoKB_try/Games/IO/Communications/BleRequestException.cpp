#include "BleRequestException.h"


using namespace Games::IO::Communications;


BleRequestException::BleRequestException(BleResponseCode bResponseCode)
{
	bleResponseCode = bResponseCode;
}

BleResponseCode BleRequestException::GetBleResponseCode()
{
	return bleResponseCode;
}