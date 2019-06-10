#include "FallMapAlgorithm.h"



using namespace Meteor::Schedulers::Events::Effects::Algorithms; 
using namespace Meteor::Schedulers::Events::Effects;
using namespace Base::Schedulers::Events::Effects;
using namespace Meteor::Config;



int FallMapAlgorithm::load()
{
	LOG(LogLevel::Info) << "FallMapAlgorithm::load() : Start loading config.";

	shiftAlgo = new MapShiftAlgorithm<FallEffect>(startX);

	MeteorConfigManager* m = GetCache<MeteorConfigManager>("MeteorConfigManager");

	if (!m)
		throw runtime_error("int MeteorPlayfield::load() : MeteorConfigManager not found in cache.");
	
	return load(m);
}

int FallMapAlgorithm::load(MeteorConfigManager * m)
{

	if (m->Get(MeteorSetting::FallLength, &fallLength)) {}

	genAlgo = new FallMapGenerateAlgorithm(m);

	return 0;
}

FallMapAlgorithm::FallMapAlgorithm(): RegisterType("FallMapAlgorithm"), MapAlgorithm()
{
	registerLoad(bind((int(FallMapAlgorithm::*)())&FallMapAlgorithm::load, this));
}

FallMapGenerateAlgorithm::FallMapGenerateAlgorithm(MeteorConfigManager* m)
{
	if (m->Get(MeteorSetting::FallLength, &fallLength)) {}
	if (m->Get(MeteorSetting::FallBrightness, &fallBrightness)) {}
}

int FallMapGenerateAlgorithm::ImplementGenerate(Map * m, EffectMapper<FallEffect>* em)
{

	LOG(LogLevel::Depricated) << "FallMapGenerateAlgorithm::ImplementGenerate() : Generate Effect [" << em->GetStartTime() << "] on [" << em->GetCurrentTime() << "].";

	MTO_FLOAT currentTime = em->GetCurrentTime();
	// MTO_FLOAT lifeTime = em->GetLifeTime();
	MTO_FLOAT speed = em->GetSpeed();

	int width = em->GetWidth();
	int height = em->GetHeight();

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
	MTO_FLOAT meteorPos = height - speed * currentTime;
	// 公式: -256*y + 256 
	// 算流星燈每一個燈泡的亮度，從下面網上算
	for (int i = 0; i < height; i++) {

		// TODO: 應該把流星長度變成參數
		if (i > meteorPos && i < meteorPos + MTO_FLOAT(fallLength)) {

			int brightness = (-BRIGHTNESS_MAX * (MTO_FLOAT(i) - meteorPos) / MTO_FLOAT(fallLength) + BRIGHTNESS_MAX) * fallBrightness;
			LOG(LogLevel::Finest) << "FallMapGenerateAlgorithm::ImplementGenerate : bright_max: " << BRIGHTNESS_MAX << ", MtoPos: " << meteorPos << ", i: " << i << ", bright: " << brightness;
			if (brightness > 0)
				m->Add(width, height + i, brightness);
		}
	}
	LOG(LogLevel::Finest) << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "FallMapGenerateAlgorithm::ImplementGenerate : light map - after";
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

	return 0;
}
