#include "GlowLineMapAlgorithm.h"

using namespace Meteor::Schedulers::Events::Effects::Algorithms;




int GlowLineMapAlgorithm::load()
{
	genAlgo = new GlowLineMapGenerateAlgorithm();
	shiftAlgo = new MapShiftAlgorithm<GlowLineEffect>(startX);
	return 0;
}

GlowLineMapAlgorithm::GlowLineMapAlgorithm(): RegisterType("GlowLineMapAlgorithm"), MapAlgorithm()
{
	registerLoad(bind((int(GlowLineMapAlgorithm::*)())&GlowLineMapAlgorithm::load, this));
}

int GlowLineMapGenerateAlgorithm::ImplementGenerate(Map * m, EffectMapper<GlowLineEffect>* em)
{

	LOG(LogLevel::Finest) << "GlowLineMapGenerateAlgorithm::ImplementGenerate() : Generate Effect [" << em->GetStartTime() << "] on [" << em->GetCurrentTime() << "].";

	// TODO: Ãi±o¼g ¤U¦¸¼g
	return 0;
}
