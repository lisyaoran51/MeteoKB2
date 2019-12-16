#include "ExplodeMapAlgorithm.h"


using namespace Meteor::Schedulers::Events::Effects::Algorithms;


int ExplodeMapAlgorithm::load()
{
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
	LOG(LogLevel::Debug) << "ExplodeMapGenerateAlgorithm::ImplementGenerate() : generate map";
	//LOG(LogLevel::Finest) << "ExplodeMapGenerateAlgorithm::ImplementGenerate() : Generate Effect [" << em->GetStartTime() << "] on [" << em->GetCurrentTime() << "].";

	int width = em->GetWidth();
	int height = em->GetHeight();

	bool isAdded = false;

	if (em->GetProcessorTimeLeft() > 0 && em->GetProcessorTimeLeft() < em->GetLifeTime()) {
		isAdded = true;
		for (int i = 0; i < height; i++) {

			// TODO: �n�令�̫�T���O�G�N�n
			m->Add(width, height + i, 1);

		}
	}

	return 0;
}
