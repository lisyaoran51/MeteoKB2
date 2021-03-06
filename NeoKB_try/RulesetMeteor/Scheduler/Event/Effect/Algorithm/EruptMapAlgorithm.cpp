#include "EruptMapAlgorithm.h"



using namespace Meteor::Schedulers::Events::Effects::Algorithms; 
using namespace Meteor::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::Effects;
using namespace Meteor::Config;



int EruptMapAlgorithm::load()
{
	LOG(LogLevel::Info) << "EruptMapAlgorithm::load() : Start loading config.";

	shiftAlgo = new MapShiftAlgorithm<EruptEffect>(startX);

	MeteorConfigManager* m = GetCache<MeteorConfigManager>("MeteorConfigManager");

	if (!m)
		throw runtime_error("int MeteorPlayfield::load() : MeteorConfigManager not found in cache.");
	
	return load(m);
}

int EruptMapAlgorithm::load(MeteorConfigManager * m)
{

	if (m->Get(MeteorSetting::FallLength, &fallLength)) {}

	genAlgo = new EruptMapGenerateAlgorithm(m);

	return 0;
}

EruptMapAlgorithm::EruptMapAlgorithm(): RegisterType("EruptMapAlgorithm"), MapAlgorithm()
{
	registerLoad(bind((int(EruptMapAlgorithm::*)())&EruptMapAlgorithm::load, this));
}

EruptMapGenerateAlgorithm::EruptMapGenerateAlgorithm(MeteorConfigManager* m)
{
	if (m->Get(MeteorSetting::FallLength, &fallLength)) {}
	if (m->Get(MeteorSetting::FallBrightness, &fallBrightness)) {}
}

int EruptMapGenerateAlgorithm::ImplementGenerate(Map * m, EffectMapper<EruptEffect>* em)
{

	//LOG(LogLevel::Depricated) << "FallMapGenerateAlgorithm::ImplementGenerate() : Generate Effect [" << em->GetStartTime() << "] on [" << em->GetCurrentTime() << "].";

	MTO_FLOAT currentTime = em->GetCurrentTime();// em->GetCurrentTime();
	MTO_FLOAT startTime = em->GetStartTime();
	if (startTime > currentTime)
		return -1;

	// MTO_FLOAT lifeTime = em->GetLifeTime();
	MTO_FLOAT speed = em->GetSpeed();

	int width = em->GetWidth();
	int height = em->GetHeight();


	bool isAdded = false;

	LOG(LogLevel::Depricated) << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "FallMapGenerateAlgorithm::ImplementGenerate : light map - before";
		// 因為只看畫面中央，所以不看其他排
		for (int i = 0; i < width * 2; i++) {
			string s;
			for (int j = 0; j < height * 2; j++) {
				s += to_string(m->Get(i, j));
				s += " ";
			}
			LOG(LogLevel::Finest) << "| " << s << "|";
		}
		return 0;
	}(width, height, m);

	// 目前流星位置：height - speed * currentTime 
	MTO_FLOAT meteorPos = speed * (currentTime - startTime) - height;
	
	LOG(LogLevel::Depricated) << "EruptMapGenerateAlgorithm::ImplementGenerate : meteorPos = " << meteorPos << ", current time = " << currentTime << ", speed = " << speed;
	// 公式: -256*y + 256 
	// 算流星燈每一個燈泡的亮度，從下面網上算
	for (int i = 0; i < height; i++) {

		// TODO: 應該把流星長度變成參數
		if (i > meteorPos - 0.5 && i < meteorPos + MTO_FLOAT(fallLength)) {

			int brightness = (-BRIGHTNESS_MAX * (MTO_FLOAT(i) - meteorPos) / MTO_FLOAT(fallLength) + BRIGHTNESS_MAX) * fallBrightness;
			if (brightness > 0) {
				LOG(LogLevel::Finest) << "EruptMapGenerateAlgorithm::ImplementGenerate : bright_max: " << BRIGHTNESS_MAX << ", MtoPos: " << meteorPos << ", i: " << i << ", bright: " << brightness;
				m->Add(width, height + i, brightness);
				isAdded = true;
			}
		}
	}

	if(isAdded)
	LOG(LogLevel::Depricated) << "EruptMapGenerateAlgorithm::ImplementGenerate : current time = " << currentTime << ", start time = " << em->GetStartTime() << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "EruptMapGenerateAlgorithm::ImplementGenerate : light map - after";
		// 因為只看畫面中央，所以不看其他排
		for (int i = 0; i < width*2; i++) {
			string s;
			for (int j = 0; j < height*2; j++) {
				s += to_string(m->Get(i, j));
				s += " ";
			}
			LOG(LogLevel::Finest) << "| " << s << "|";
		}
		return 0;
	}(width, height, m);


	return isAdded ? 0 : -1;
}
