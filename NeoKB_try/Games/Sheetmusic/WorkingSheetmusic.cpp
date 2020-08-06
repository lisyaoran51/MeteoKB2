#include "WorkingSheetmusic.h"

#include "../../Framework/Audio/Track/RateSettableBassTrack.h"
#include <string.h>


using namespace Games::Sheetmusics;
using namespace Games::Sheetmusics::Format;
using namespace Framework::Audio::Tracks;
using namespace std::literals::string_literals;
using namespace std;




WorkingSm::WorkingSm(SmInfo * s)
{

	smInfo = s;
	if (s->metadata)
		smMetadata = new SmMetadata(s->metadata);
	else
		smMetadata = new SmMetadata();

	modifiers = new BindablePointer<vector<Modifier*>*>(new vector<Modifier*>());
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

bool WorkingSm::IsTheSameSm(SmInfo * s)
{
	// TODO: not implemented
	return false;
}

BindablePointer<vector<Modifier*>*>* WorkingSm::GetModifiers()
{
	return modifiers;
}

Track * WorkingSm::GetTrack()
{
	if (track != nullptr)
		return track;

	LOG(LogLevel::Debug) << "WorkingSm::GetTrack() : finding track name [" << smInfo->metadata->AudioFile << "].";
	LOG(LogLevel::Error) << "WorkingSm::GetTrack() : here is a bug not solved.";
	//ifstream* stream = new ifstream(getPathForFile(smInfo->metadata->AudioFile));

	if (smInfo->metadata->AudioFile == "")
		return nullptr;

	string path = getPathForFile(smInfo->metadata->AudioFile);
	char * temp = new char[path.length()];
	LOG(LogLevel::Debug) << "WorkingSm::GetTrack() : new a char array.";
	strcpy(temp, path.c_str());
	LOG(LogLevel::Debug) << "WorkingSm::GetTrack() : get file path = [" << temp << "].";
	track = new RateSettableBassTrack(temp);
	LOG(LogLevel::Debug) << "WorkingSm::GetTrack() : end].";
	return track;
}

Sm<Event>* WorkingSm::createSm()
{
	Sm<Event>* s;

	// get path for file是在這個sm set裡面找他們全部的path，然後把path和檔名家在一起，回傳
	//fstream* stream = new fstream(getPathForFile(smInfo->fileName));

	LOG(LogLevel::Finer) << "WorkingSm::createSm() : decoding [" << getPathForFile(smInfo->fileName) << "].";
	
	fstream* stream = new fstream(getPathForFile(smInfo->fileName)); // 暫時先這樣寫


	SmDecoder* smDecoder = SmDecoder::GetDecoder(stream);
	s = smDecoder->Decode(stream);

	return s;
}

string WorkingSm::getPathForFile(string fileName)
{
	string s = smInfo->fileInfo->Path + "/"s + fileName;
	LOG(LogLevel::Finer) << "string WorkingSm::getPathForFile(string) : creating Sm from path [" << s << "] ...";
	return s;
}

