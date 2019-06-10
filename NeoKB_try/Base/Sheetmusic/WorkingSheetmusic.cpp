#include "WorkingSheetmusic.h"


using namespace Base::Sheetmusics;
using namespace Base::Sheetmusics::Format;


WorkingSm::WorkingSm(SmInfo * s)
{

	smInfo = s;
	if (s->metadata)
		smMetadata = new SmMetadata(s->metadata);
	else
		smMetadata = new SmMetadata();
}

WorkingSm::~WorkingSm()
{
	// 確定都是這邊來山嗎??
	delete sm;
	delete smMetadata;
	delete smInfo;
}

Sm<Event>* WorkingSm::GetSm()
{
	if (sm) {
		LOG(LogLevel::Fine) << "Sm<Event>* WorkingSm::GetSm() : retrieving existing sm ...";
		return sm;
	}

	LOG(LogLevel::Fine) << "Sm<Event>* WorkingSm::GetSm() : creating Sm ...";
	sm = createSm();

	// use the database-backed info.
	sm->SetSmInfo(smInfo);

	return sm;
}

Sm<Event>* WorkingSm::createSm()
{
	Sm<Event>* s;

	// get path for file是在這個sm set裡面找他們全部的path，然後把path和檔名家在一起，回傳
	ifstream* stream = new ifstream(getPathForFile(smInfo->fileName));

	SmDecoder* smDecoder = SmDecoder::GetDecoder(stream);
	s = smDecoder->Decode(stream);

	return s;
}

string WorkingSm::getPathForFile(string fileName)
{
	string s = smInfo->smSetInfo->path + "/" + fileName;
	LOG(LogLevel::Finer) << "string WorkingSm::getPathForFile(string) : creating Sm from path [" << s << "] ...";
	return s;
}

