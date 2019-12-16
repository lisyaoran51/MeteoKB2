#include "ExplodeMapAlgorithm.h"


using namespace Meteor::Schedulers::Events::Effects::Algorithms;


int ExplodeMapAlgorithm::load()
{
	// 如果要改Explosion設定，要從這邊改
	genAlgo = new ExplodeMapGenerateAlgorithm();
	shiftAlgo = new MapShiftAlgorithm<ExplodeEffect>(startX);
	return 0;
}

ExplodeMapAlgorithm::ExplodeMapAlgorithm(): RegisterType("ExplodeMapAlgorithm"), MapAlgorithm()
{
	registerLoad(bind((int(ExplodeMapAlgorithm::*)())&ExplodeMapAlgorithm::load, this));
}

int ExplodeMapGenerateAlgorithm::ImplementGenerate(Map * m, EffectMapper<ExplodeEffect>* em)
{
	
	//LOG(LogLevel::Finest) << "ExplodeMapGenerateAlgorithm::ImplementGenerate() : Generate Effect [" << em->GetStartTime() << "] on [" << em->GetCurrentTime() << "].";

	int width = em->GetWidth();
	int height = em->GetHeight();
	//LOG(LogLevel::Debug) << "ExplodeMapGenerateAlgorithm::ImplementGenerate() : generate map " << m;
	bool isAdded = false;

	//LOG(LogLevel::Debug) << "ExplodeMapGenerateAlgorithm::ImplementGenerate() : GetProcessorTimeLeft " << em->GetProcessorTimeLeft() << ", GetLifeTime " << em->GetLifeTime();

	if (em->GetProcessorTimeLeft() > 0 && em->GetProcessorTimeLeft() < em->GetLifeTime()) {
		isAdded = true;
		for (int i = 0; i < height; i++) {

			// TODO: 要改成最後三顆燈亮就好
			m->Add(width, height + i, 1);

		}
	}

	//LOG(LogLevel::Debug) << "ExplodeMapGenerateAlgorithm::ImplementGenerate() : generate map end.";

	return isAdded ? 0 : -1;
}
