#include "RecordPatternGenerator.h"




using namespace Record::Sheetmusics::Patterns;
using namespace Record::Config;



int RecordPatternGenerator::load()
{

	RecordConfigManager * r = GetCache<RecordConfigManager>("RecordConfigManager");

	if (!r)
		throw runtime_error("int RecordPatternGenerator::load() : RecordConfigManager not found in cache.");

	return load(r);
}

int RecordPatternGenerator::load(RecordConfigManager * i)
{

	return 0;
}

RecordPatternGenerator::RecordPatternGenerator(): PatternGenerator(), RegisterType("RecordPatternGenerator")
{
	registerLoad(bind(static_cast<int(RecordPatternGenerator::*)(void)>(&RecordPatternGenerator::load), this));
}

int RecordPatternGenerator::Initialize(Sm<Event>* s)
{
	PatternGenerator::Initialize(s);

	return 0;
}

Pattern* RecordPatternGenerator::Generate(vector<Event*>* es, Event * e)
{
	
	return nullptr;

}

int RecordPatternGenerator::CreateOtherEvent(vector<Event*>* es)
{
	
	return 0;
}

int RecordPatternGenerator::PostProcess()
{
	for (int i = 0; i < patterns.size(); i++) {



	}



	return 0;
}

