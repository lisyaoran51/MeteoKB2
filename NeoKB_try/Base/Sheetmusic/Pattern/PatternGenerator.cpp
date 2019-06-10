#include "PatternGenerator.h"


using namespace Base::Sheetmusics::Patterns;
using namespace std;




int PatternGenerator::load()
{
	FrameworkConfigManager * f = GetCache<FrameworkConfigManager>("FrameworkConfigManager");
	return load(f);
}

int PatternGenerator::load(FrameworkConfigManager * fConfigManager)
{
	if(fConfigManager->Get(FrameworkSetting::Width, &width));
	if(fConfigManager->Get(FrameworkSetting::Height, &height));
	if(fConfigManager->Get(FrameworkSetting::TargetHeight, &targetHeight));
	if(fConfigManager->Get(FrameworkSetting::BlackKeyHeight, &blackKeyHeight));
	if(fConfigManager->Get(FrameworkSetting::BlackKeyTargetHeight, &blackKeyTargetHeight));
	if(fConfigManager->Get(FrameworkSetting::StartPitch, (int*)&startPitch));
	return 0;
}

PatternGenerator::PatternGenerator(): ChildAddable(), RegisterType("PatternGenerator")
{
	registerLoad(bind(static_cast<int(PatternGenerator::*)(void)>(&PatternGenerator::load), this));
}

PatternGenerator::~PatternGenerator()
{
	CleanPatterns();
}

int PatternGenerator::Add(Pattern * p)
{

	patterns.push_back(p);

	return 0;
}

int PatternGenerator::CleanPatterns()
{
	for (int i = 0; i < patterns.size(); i++) {
		delete patterns[i];
	}
	return 0;
}

int PatternGenerator::CreateOtherEvent(vector<Event*>* es)
{
	return -1;
}
