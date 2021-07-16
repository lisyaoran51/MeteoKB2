#include "InstantSpotMapAlgorithm.h"

#include "../../../../../Games/Scene/Play/Playfield.h"
#include "../../../../Scenes/Play/InstantPlayfield.h"


using namespace Instant::Schedulers::Events::Effects::Algorithms;
using namespace Games::Scenes::Play;
using namespace Instant::Scenes::Play;


int InstantSpotMapAlgorithm::load()
{
	InstantSpotMapGenerateAlgorithm* newGenAlgo = new InstantSpotMapGenerateAlgorithm();
	newGenAlgo->SetPlayfield(playfield);
	genAlgo = newGenAlgo;
	shiftAlgo = new MapShiftAlgorithm<InstantSpotEffect>(startX);


	return 0;
}

InstantSpotMapAlgorithm::InstantSpotMapAlgorithm() : RegisterType("InstantSpotMapAlgorithm"), MapAlgorithm()
{
	registerLoad(bind((int(InstantSpotMapAlgorithm::*)())&InstantSpotMapAlgorithm::load, this));
}

int InstantSpotMapAlgorithm::SetPlayfield(Playfield * p)
{
	playfield = p;
	return 0;
}

int InstantSpotMapGenerateAlgorithm::SetPlayfield(Playfield * p)
{
	playfield = p;
	return 0;
}

int InstantSpotMapGenerateAlgorithm::ImplementGenerate(Map * m, EffectMapper<InstantSpotEffect>* em)
{

	MTO_FLOAT currentTime = em->GetCurrentTime();// em->GetCurrentTime();
	MTO_FLOAT startTime = em->GetStartTime();
	if (startTime > currentTime)
		return -1;

	MTO_FLOAT lifeTime = em->GetLifeTime();
	if (startTime + lifeTime < currentTime)
		return -1;



	int width = em->GetWidth();
	int height = em->GetHeight();

	if (dynamic_cast<InstantSpotEffect*>(em->GetEvent())->GetEffectPinType() != EffectPinType::ByPitch) {
		LOG(LogLevel::Warning) << "InstantFallMapGenerateAlgorithm::ImplementGenerate : this effect not by pitch";
		return -1;
	}

	int xShiftLength = 0;
	switch (dynamic_cast<InstantPlayfield*>(playfield)->GetMeteoPianoPitchState()) {
	case MeteoPianoPitchState::Lowered:
		xShiftLength = 12;
		break;
	case MeteoPianoPitchState::Raised:
		xShiftLength = -12;
		break;
	}


	bool isAdded = false;

	LOG(LogLevel::Depricated) << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "InstantGlowLineMapGenerateAlgorithm::ImplementGenerate : light map - before";
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

	int spotYPosition = 0;
	switch (em->GetX() % 12) {
	case 1:
	case 3:
	case 6:
	case 8:
	case 10:
		spotYPosition = 5;
		break;
	}

	// 公式: -256*y + 256 
	// 算流星燈每一個燈泡的亮度，從下面網上算
	//for (int i = 0; i < height; i++) {
	//
	//	m->Add(width + xShiftLength, height + spotYPosition, 1);
	//
	//}
	m->Add(width + xShiftLength, height + spotYPosition, 1);
	isAdded = true;

	if (isAdded)
		LOG(LogLevel::Depricated) << "InstantGlowLineMapGenerateAlgorithm::ImplementGenerate : current time = " << currentTime << ", start time = " << em->GetStartTime() << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "InstantGlowLineMapGenerateAlgorithm::ImplementGenerate : light map - after";
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


	return isAdded ? 0 : -1;
}
