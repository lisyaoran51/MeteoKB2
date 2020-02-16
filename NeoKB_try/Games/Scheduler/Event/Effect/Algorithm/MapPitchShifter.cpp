#include "MapPitchShifter.h"


using namespace Games::Schedulers::Events::Effects::Algorithms;




int MapPitchShifter::load()
{
	LOG(LogLevel::Info) << "MapPitchShifter::load() : Start loading config.";

	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

	if (!f)
		throw runtime_error("int MapPitchShifter::load() : FrameworkConfigManager not found in cache.");

	return load(f);
}

int MapPitchShifter::load(FrameworkConfigManager * f)
{
	int startPitch;
	if (!f->Get(FrameworkSetting::StartPitch, &startPitch))
		throw runtime_error("int MapPitchShifter::load() : start pitch not found in FrameworkConfigManager.");
	defaultStartPitch = (Pitch)startPitch;
	upperBound = (Pitch)(startPitch + 12);
	lowerBound = (Pitch)(startPitch - 12);

	if (!f->Get(FrameworkSetting::Width, &width)) 
		throw runtime_error("int MapPitchShifter::load() : width not found in FrameworkConfigManager.");
	if (!f->Get(FrameworkSetting::Height, &height)) 
		throw runtime_error("int MapPitchShifter::load() : height not found in FrameworkConfigManager.");

	return 0;
}

MapPitchShifter::MapPitchShifter(): RegisterType("MapPitchShifter")
{

	registerLoad(bind((int(MapPitchShifter::*)())&MapPitchShifter::load, this));

}

int MapPitchShifter::LazyConstruct(map<string, MapAlgorithmInterface*>* mapAlgorithms)
{
	mapAlgos = mapAlgorithms;
	isConstructed = true;
	return 0;
}

bool MapPitchShifter::GetIsConstructed()
{

	return isConstructed;
}

int MapPitchShifter::update()
{
	if (shift() == RETURN_AVAILABLE) {

		map<string, MapAlgorithmInterface*>::iterator iter;
		for (iter = mapAlgos->begin(); iter != mapAlgos->end(); iter++) {
			iter->second->SetStartX((int)GetTempPitch());
		}
		LOG(LogLevel::Depricated) << "MapPitchShifter::update() : shift algos ";
	}
	return 0;
}
