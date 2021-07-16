#include "InstantFallMapAlgorithm.h"

#include "../../../../../Games/Scene/Play/Playfield.h"


using namespace Instant::Schedulers::Events::Effects::Algorithms;
using namespace Instant::Schedulers::Events::Effects;
using namespace Games::Schedulers::Events::Effects;
using namespace Instant::Config;
using namespace Games::Scenes::Play;



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

	InstantFallMapGenerateAlgorithm* newGenAlgo = new InstantFallMapGenerateAlgorithm(m);

	newGenAlgo->SetPlayfield(playfield);

	genAlgo = newGenAlgo;

	return 0;
}

InstantFallMapAlgorithm::InstantFallMapAlgorithm(): RegisterType("InstantFallMapAlgorithm"), MapAlgorithm()
{
	registerLoad(bind((int(InstantFallMapAlgorithm::*)())&InstantFallMapAlgorithm::load, this));
}

int InstantFallMapAlgorithm::SetPlayfield(Playfield * p)
{
	playfield = p;
	return 0;
}

InstantFallMapGenerateAlgorithm::InstantFallMapGenerateAlgorithm(InstantConfigManager* m)
{
	if (m->Get(InstantSetting::FallLength, &fallLength)) {}
	if (m->Get(InstantSetting::FallBrightness, &fallBrightness)) {}
}

int InstantFallMapGenerateAlgorithm::SetPlayfield(Playfield * p)
{
	playfield = p;
	return 0;
}

int InstantFallMapGenerateAlgorithm::ImplementGenerate(Map * m, EffectMapper<InstantFallEffect>* em)
{
	LOG(LogLevel::Depricated) << "InstantFallMapGenerateAlgorithm::ImplementGenerate() : Generate Effect [" << em->GetStartTime() << "] on [" << em->GetCurrentTime() << "].";

	MTO_FLOAT currentTime = em->GetCurrentTime();// em->GetCurrentTime();
	MTO_FLOAT startTime = em->GetStartTime();
	MTO_FLOAT lifeTime = em->GetLifeTime();
	if (startTime > currentTime)
		return -1;

	if (startTime + lifeTime + 0.1 < currentTime)	// 0.1是buffer，以免常常還沒亮到登調尾巴，就熄滅了
		return -1;

	// MTO_FLOAT lifeTime = em->GetLifeTime();
	MTO_FLOAT speed = em->GetSpeed();

	int width = em->GetWidth();
	int height = em->GetHeight();

	if (dynamic_cast<InstantFallEffect*>(em->GetEvent())->GetEffectPinType() != EffectPinType::ByPitch) {
		LOG(LogLevel::Warning) << "InstantFallMapGenerateAlgorithm::ImplementGenerate : this effect not by pitch";
		return -1;
	}

	int xPosition = playfield->GetXPositionFromPitch(dynamic_cast<InstantFallEffect*>(em->GetEvent())->GetPitch());
	LOG(LogLevel::Debug) << "InstantFallMapGenerateAlgorithm::ImplementGenerate : x position is " << xPosition;

	if (xPosition < 0 || xPosition >= width)
		return -1;

	bool isAdded = false;

	LOG(LogLevel::Depricated) << [](int width, int height, Map* m) {
		LOG(LogLevel::Finest) << "InstantFallMapGenerateAlgorithm::ImplementGenerate : light map - before";
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

	/* 流光運作方式

	16 --- start ---
		|		  |
		|		  |	位置
		|		  |
		O   now	 ---
	   ||
	   v|
		|
		|
	0  ---

	*/

	// 目前流星位置：height - speed * currentTime 
	MTO_FLOAT meteorPos = height - speed * (currentTime - startTime);

	LOG(LogLevel::Depricated) << "InstantFallMapGenerateAlgorithm::ImplementGenerate : meteorPos = " << meteorPos << ", current time = " << currentTime << ", speed = " << speed;
	// 公式: -256*y + 256 
	// 算流星燈每一個燈泡的亮度，從下面網上算
	for (int i = 0; i < height; i++) {

		/* 新版fall algo */
		if (i > meteorPos - 0.5 && i <= meteorPos + 0.5) {
			m->Add(width + xPosition, height + i, 1);
			isAdded = true;
			break;
		}
		continue;
	}

	if (isAdded)
		LOG(LogLevel::Debug) << "InstantFallMapGenerateAlgorithm::ImplementGenerate : current time = " << currentTime << ", start time = " << em->GetStartTime() << [](int width, int height, Map* m) {
		LOG(LogLevel::Debug) << "InstantFallMapGenerateAlgorithm::ImplementGenerate : light map - after";
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
