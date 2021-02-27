#include"RecordSheetmusicConverter.h"





using namespace Record::Sheetmusics;



RecordSmConverter::RecordSmConverter(PatternGenerator * pg): SmConverter(pg)
{
}

Sm<Event>* RecordSmConverter::Convert(Sm<Event>* s)
{
	LOG(LogLevel::Fine) << "MeteorSmConverter::Convert(Sm<Event>*) : Start converting sheetmusic...";
	Sm<Event>* convertedSm = SmConverter::Convert(s);


	return convertedSm;
}

int RecordSmConverter::convertEvent(vector<Event*>* es, Event* e)
{


	return 0;
}
