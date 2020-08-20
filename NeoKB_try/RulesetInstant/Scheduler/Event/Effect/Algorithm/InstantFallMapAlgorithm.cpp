#include "InstantFallMapAlgorithm.h"



using namespace Instant::Schedulers::Events::Effects::Algorithms;
using namespace Instant::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::Effects;
using namespace Instant::Config;



int InstantFallMapAlgorithm::load()
{
	LOG(LogLevel::Info) << "FallMapAlgorithm::load() : Start loading config.";

	shiftAlgo = new MapShiftAlgorithm<InstantFallEffect>(startX);

	InstantConfigManager* m = GetCache<InstantConfigManager>("InstantConfigManager");

	if (!m)
		throw runtime_error("int MeteorPlayfield::load() : MeteorConfigManager not found in cache.");
	
	return load(m);
}

int InstantFallMapAlgorithm::load(InstantConfigManager * m)
{

	if (m->Get(InstantSetting::FallLength, &fallLength)) {}

	genAlgo = new InstantFallMapGenerateAlgorithm(m);

	return 0;
}

InstantFallMapAlgorithm::InstantFallMapAlgorithm(): RegisterType("InstantFallMapAlgorithm"), MapAlgorithm()
{
	registerLoad(bind((int(InstantFallMapAlgorithm::*)())&InstantFallMapAlgorithm::load, this));
}

InstantFallMapGenerateAlgorithm::InstantFallMapGenerateAlgorithm(InstantConfigManager* m)
{
	if (m->Get(InstantSetting::FallLength, &fallLength)) {}
	if (m->Get(InstantSetting::FallBrightness, &fallBrightness)) {}
}

int InstantFallMapGenerateAlgorithm::ImplementGenerate(Map * m, EffectMapper<InstantFallEffect>* em)
{

	LOG(LogLevel::Depricated) << "FallMapGenerateAlgorithm::ImplementGenerate() : Generate Effect [" << em->GetStartTime() << "] on [" << em->GetCurrentTime() << "].";

	MTO_FLOAT currentTime = em->GetCurrentTime();// em->GetCurrentTime();
	MTO_FLOAT startTime = em->GetStartTime();
	// �o�Ӹ�meteor fall effect���P���a��O�A�o�Ӫ�start time�O���U���������Ameteor��start time�O��F�������ɶ�
	if (startTime < currentTime)
		return -1;

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
	MTO_FLOAT meteorPos = speed * (startTime - currentTime);
	
	LOG(LogLevel::Depricated) << "FallMapGenerateAlgorithm::ImplementGenerate : meteorPos = " << meteorPos << ", current time = " << currentTime << ", speed = " << speed;
	// ����: -256*y + 256 
	// ��y�P�O�C�@�ӿO�w���G�סA�q�U�����W��
	for (int i = 0; i < height; i++) {

		// TODO: ���ӧ�y�P�����ܦ��Ѽ�
		if (i > meteorPos - 0.5 && i < meteorPos + MTO_FLOAT(fallLength)) {

			int brightness = (-BRIGHTNESS_MAX * (MTO_FLOAT(i) - meteorPos) / MTO_FLOAT(fallLength) + BRIGHTNESS_MAX) * fallBrightness;
			if (brightness > 0) {
				LOG(LogLevel::Finest) << "FallMapGenerateAlgorithm::ImplementGenerate : bright_max: " << BRIGHTNESS_MAX << ", MtoPos: " << meteorPos << ", i: " << i << ", bright: " << brightness;
				m->Add(width, height + i, brightness);
				isAdded = true;
			}
		}
	}

	if(isAdded)
	LOG(LogLevel::Depricated) << "FallMapGenerateAlgorithm::ImplementGenerate : current time = " << currentTime << ", start time = " << em->GetStartTime() << [](int width, int height, Map* m) {
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


	return isAdded ? 0 : -1;
}
