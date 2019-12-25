#include "SustainPedalLightRingIoCommunicator.h"



using namespace Meteor::Schedulers::Events::IoEvents::IoCommunicators;



int SustainPedalLightRingIoCommunicator::load()
{

	LOG(LogLevel::Info) << "SustainPedalLightRingIoCommunicator::load() : Start loading.";

	MainInterface* m = GetCache<MainInterface>("MainInterface");

	if (!m)
		throw runtime_error("int IoCommunicator::load() : FrameworkConfigManager not found in cache.");

	return load(m);
}

int SustainPedalLightRingIoCommunicator::load(MainInterface * m)
{
	return 0;
}

SustainPedalLightRingIoCommunicator::SustainPedalLightRingIoCommunicator():RegisterType("SustainPedalLightRingIoCommunicator")
{
	registerLoad(bind((int(SustainPedalLightRingIoCommunicator::*)())&SustainPedalLightRingIoCommunicator::load, this));
}

int SustainPedalLightRingIoCommunicator::RegisterIoPeripheral(Peripheral* ioPeripheral)
{
	IoCommunicator::RegisterIoPeripheral(ioPeripheral);

	if (dynamic_cast<Panel*>(ioPeripheral)) {
		panel = dynamic_cast<Panel*>(ioPeripheral);
	}
	else {
		// throw error
	}

	return 0;
}

int SustainPedalLightRingIoCommunicator::implementProcessIO(SustainPedalLightRing * sPedalLightRing)
{
	if (!panel)
		return -1;




	return 0;
}
