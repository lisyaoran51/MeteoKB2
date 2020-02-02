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
	// �T�w���O�o��Ӥs��??
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
	//ifstream* stream = new ifstream(getPathForFile(smInfo->metadata->AudioFile));
	string path = getPathForFile(smInfo->metadata->AudioFile);
	char * temp = new char[path.length()];
	strcpy(temp, path.c_str());
	LOG(LogLevel::Finer) << "WorkingSm::GetTrack() : get file path = [" << temp << "].";
	track = new RateSettableBassTrack(temp);
	return track;
}

Sm<Event>* WorkingSm::createSm()
{
	Sm<Event>* s;

	// get path for file�O�b�o��sm set�̭���L�̥�����path�A�M���path�M�ɦW�a�b�@�_�A�^��
	//ifstream* stream = new ifstream(getPathForFile(smInfo->fileName));

	LOG(LogLevel::Finer) << "WorkingSm::createSm() : decoding [" << getPathForFile(smInfo->fileName) << "].";
	
	ifstream* stream = new ifstream(getPathForFile(smInfo->fileName)); // �Ȯɥ��o�˼g


	SmDecoder* smDecoder = SmDecoder::GetDecoder(stream);
	s = smDecoder->Decode(stream);

	return s;
}

string WorkingSm::getPathForFile(string fileName)
{
	string s = smInfo->fileInfo->Path + "/"s + fileName;
	LOG(LogLevel::Fine) << "string WorkingSm::getPathForFile(string) : creating Sm from path [" << s << "] ...";
	return s;
}

