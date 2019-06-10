#include "MeteorPlayfield.h"

#include "../../Base/Scheduler/Event/Effect/Algorithm/MapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/ExplodeMapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/FallMapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/GlowLineMapAlgorithm.h"
#include "../../RulesetMeteor/Scheduler/Event/Effect/Algorithm/TargetLineMapAlgorithm.h"
#include "../../Util/Log.h"



using namespace Meteor::Play;
using namespace Meteor::Config;
using namespace Base::Schedulers::Events::Effects::Algorithms;
using namespace Meteor::Schedulers::Events::Effects::Algorithms;
using namespace Util;


int MeteorPlayfield::load()
{
	LOG(LogLevel::Info) << "MeteorPlayfield::load() : Start loading game scene.";

	MeteorConfigManager* m = GetCache<MeteorConfigManager>("MeteorConfigManager");

	if (!m)
		throw runtime_error("int MeteorPlayfield::load() : MeteorConfigManager not found in cache.");

	FrameworkConfigManager* f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");

	if (!f)
		throw runtime_error("int MeteorPlayfield::load() : FrameworkConfigManager not found in cache.");

	return load(f, m);
}

int MeteorPlayfield::load(FrameworkConfigManager* f, MeteorConfigManager * m)
{
	if(f->Get(FrameworkSetting::StartPitch, (int*)&startPitch)){}
	if(f->Get(FrameworkSetting::Width, &pitchCount)){}


	/* 利用map algo的名字建立map algo */
	InstanceCreator<MtoObject> &iCreator = InstanceCreator<MtoObject>::GetInstance();
	string mapAlgoName;

	/* --------------------- FallEffect map algo --------------------- */
	if (m->Get(MeteorSetting::FallMapAlgorithm, &mapAlgoName)) {
		MapAlgorithm<Event>* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithm<Event>>(mapAlgoName);

		mapAlgorithms["FallEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["FallEffect"] = new FallMapAlgorithm();

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : FallMapAlgorithm [" << mapAlgorithms["FallEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["FallEffect"]);
	mapAlgorithms["FallEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- ExplodeEffect map algo --------------------- */
	if (m->Get(MeteorSetting::ExplodeMapAlgorithm, &mapAlgoName)) {
		MapAlgorithm<Event>* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithm<Event>>(mapAlgoName);

		mapAlgorithms["ExplodeEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["ExplodeEffect"] = new ExplodeMapAlgorithm();

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : ExplodeMapAlgorithm [" << mapAlgorithms["ExplodeEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["ExplodeEffect"]);
	mapAlgorithms["ExplodeEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- GlowLineEffect map algo --------------------- */
	if (m->Get(MeteorSetting::GlowLineMapAlgorithm, &mapAlgoName)) {
		MapAlgorithm<Event>* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithm<Event>>(mapAlgoName);

		mapAlgorithms["GlowLineEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["GlowLineEffect"] = new GlowLineMapAlgorithm();

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : GlowLineMapAlgorithm [" << mapAlgorithms["GlowLineEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["GlowLineEffect"]);
	mapAlgorithms["GlowLineEffect"]->RegisterBufferMap(bufferMap);

	/* --------------------- TargetLineEffect map algo --------------------- */
	if (m->Get(MeteorSetting::TargetLineMapAlgorithm, &mapAlgoName)) {
		MapAlgorithm<Event>* mapAlgo = iCreator.CreateInstanceWithT<MapAlgorithm<Event>>(mapAlgoName);

		mapAlgorithms["TargetLineEffect"] = mapAlgo;
	}
	else
		mapAlgorithms["TargetLineEffect"] = new TargetLineMapAlgorithm();

	LOG(LogLevel::Finer) << "MeteorPlayfield::load() : TargetLineMapAlgorithm [" << mapAlgorithms["TargetLineEffect"]->GetTypeName() << "] loaded.";

	AddChild(mapAlgorithms["TargetLineEffect"]);
	mapAlgorithms["TargetLineEffect"]->RegisterBufferMap(bufferMap);

	return 0;
}

MeteorPlayfield::MeteorPlayfield(): Playfield(), RegisterType("MeteorPlayfield")
{
	// functional裡面的bind不能對overloading問題
	// https://stackoverflow.com/questions/4159487/stdbind-overload-resolution
	registerLoad(bind((int(MeteorPlayfield::*)())&MeteorPlayfield::load, this));
}

int MeteorPlayfield::Elapse(MTO_FLOAT elapsedTime)
{
	// ???
	// playfield幹嘛要更新?
	// 應該是要抓輸入，然後去刪effect和建explode
	return 0;
}


