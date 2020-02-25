#include "ExplodeMapAlgorithm.h"


using namespace Meteor::Schedulers::Events::Effects::Algorithms;


int ExplodeMapAlgorithm::load()
{

	// �p�G�n��Explosion�]�w�A�n�q�o���
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

	LOG(LogLevel::Depricated) << "ExplodeMapGenerateAlgorithm::ImplementGenerate() : GetProcessorTimeLeft " << em->GetProcessorTimeLeft() << ", GetLifeTime " << em->GetLifeTime();

	if (em->GetTimeLeft() > 0 && em->GetTimeLeft() < em->GetLifeTime()) {
		LOG(LogLevel::Depricated) << "ExplodeMapGenerateAlgorithm::ImplementGenerate() : draw explosion.";
		isAdded = true;
		//double lightPortion = 0.25;
		for (int i = 0; i < height; i++) {
			//if ((double)i / (double)height < lightPortion) {
				// TODO: �n�令�̫�T���O�G�N�n
				m->Add(width, height + i, 1);
			//}

		}
	}

	//LOG(LogLevel::Debug) << "ExplodeMapGenerateAlgorithm::ImplementGenerate() : generate map end.";

	return isAdded ? 0 : -1;
}
