#include "FallMapAlgorithm.h"



using namespace Meteor::Schedulers::Events::Effects::Algorithms; 
using namespace Meteor::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::Effects;
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

	//LOG(LogLevel::Depricated) << "FallMapGenerateAlgorithm::ImplementGenerate() : Generate Effect [" << em->GetStartTime() << "] on [" << em->GetCurrentTime() << "].";

	MTO_FLOAT currentTime = em->GetCurrentTime();// em->GetCurrentTime();
	MTO_FLOAT startTime = em->GetStartTime();
	// MTO_FLOAT lifeTime = em->GetLifeTime();
	MTO_FLOAT speed = em->GetSpeed();

	int width = em->GetWidth();
	int height = em->GetHeight();


	bool isAdded = false;

	LOG(LogLevel::Depricated) << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "FallMapGenerateAlgorithm::ImplementGenerate : light map - before";
		// �]���u�ݵe�������A�ҥH���ݨ�L��
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

	// �ثe�y�P��m�Gheight - speed * currentTime 
	MTO_FLOAT meteorPos = height - speed * (currentTime - startTime);
	
	LOG(LogLevel::Finest) << "FallMapGenerateAlgorithm::ImplementGenerate : meteorPos = " << meteorPos << ", current time = " << currentTime << ", speed = " << speed;
	// ����: -256*y + 256 
	// ��y�P�O�C�@�ӿO�w���G�סA�q�U�����W��
	for (int i = 0; i < height; i++) {

		// TODO: ���ӧ�y�P�����ܦ��Ѽ�
		if (i > meteorPos && i < meteorPos + MTO_FLOAT(fallLength)) {

			int brightness = (-BRIGHTNESS_MAX * (MTO_FLOAT(i) - meteorPos) / MTO_FLOAT(fallLength) + BRIGHTNESS_MAX) * fallBrightness;
			if (brightness > 0) {
				LOG(LogLevel::Finer) << "FallMapGenerateAlgorithm::ImplementGenerate : bright_max: " << BRIGHTNESS_MAX << ", MtoPos: " << meteorPos << ", i: " << i << ", bright: " << brightness;
				m->Add(width, height + i, brightness);
				isAdded = true;
			}
		}
	}

	if(isAdded)
	LOG(LogLevel::Finest) << "FallMapGenerateAlgorithm::ImplementGenerate : current time = " << currentTime << ", start time = " << em->GetStartTime() << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "FallMapGenerateAlgorithm::ImplementGenerate : light map - after";
		// �]���u�ݵe�������A�ҥH���ݨ�L��
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
