#include "HardwareMapAlgorithm.h"

using namespace Base::Graphic::Renderers::MapAlgorithms;


int HardwareMapAlgo::setCompatibleHwVersion()
{
	return 0;
}

HardwareMapAlgo::HardwareMapAlgo(): RegisterType("HardwareMapAlgo")
{
	setCompatibleHwVersion();
}

int HardwareMapAlgo::RegisterMap(Map * m)
{
	lightMap = m;
	return 0;
}

bool HardwareMapAlgo::CheckHardwareVersion(int hwVersion)
{
	for (int i = 0; i < compatibleHwVersion.size(); i++) {
		if (compatibleHwVersion[i] == hwVersion)
			return true;
	}
	return false;
}
