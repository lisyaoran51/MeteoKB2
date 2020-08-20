#include "InstantGlowLineMapAlgorithm.h"

using namespace Instant::Schedulers::Events::Effects::Algorithms;




int InstantGlowLineMapAlgorithm::load()
{
	genAlgo = new InstantGlowLineMapGenerateAlgorithm();
	shiftAlgo = new MapShiftAlgorithm<InstantGlowLineEffect>(startX);
	return 0;
}

InstantGlowLineMapAlgorithm::InstantGlowLineMapAlgorithm(): RegisterType("InstantGlowLineMapAlgorithm"), MapAlgorithm()
{
	registerLoad(bind((int(InstantGlowLineMapAlgorithm::*)())&InstantGlowLineMapAlgorithm::load, this));
}

int InstantGlowLineMapGenerateAlgorithm::ImplementGenerate(Map * m, EffectMapper<InstantGlowLineEffect>* em)
{

	MTO_FLOAT currentTime = em->GetCurrentTime();// em->GetCurrentTime();
	MTO_FLOAT startTime = em->GetStartTime();
	if (startTime < currentTime)
		return -1;

	MTO_FLOAT lifeTime = em->GetLifeTime();
	if (startTime + lifeTime > currentTime)
		return -1;



	int width = em->GetWidth();
	int height = em->GetHeight();


	bool isAdded = false;

	LOG(LogLevel::Depricated) << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "InstantGlowLineMapGenerateAlgorithm::ImplementGenerate : light map - before";
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


	LOG(LogLevel::Depricated) << "InstantGlowLineMapGenerateAlgorithm::ImplementGenerate : meteorPos = " << meteorPos << ", current time = " << currentTime << ", speed = " << speed;
	// ����: -256*y + 256 
	// ��y�P�O�C�@�ӿO�w���G�סA�q�U�����W��
	for (int i = 0; i < height; i++) {

		m->Add(width, height + i, 1);

	}

	if (isAdded)
		LOG(LogLevel::Depricated) << "InstantGlowLineMapGenerateAlgorithm::ImplementGenerate : current time = " << currentTime << ", start time = " << em->GetStartTime() << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "InstantGlowLineMapGenerateAlgorithm::ImplementGenerate : light map - after";
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


	return isAdded ? 0 : -1;
}
