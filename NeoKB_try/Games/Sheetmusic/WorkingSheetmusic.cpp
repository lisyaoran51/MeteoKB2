#include "WorkingSheetmusic.h"

#include "../../Framework/Audio/Track/BassTrack.h"


using namespace Games::Sheetmusics;
using namespace Games::Sheetmusics::Format;
using namespace Framework::Audio::Tracks;



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
	// TODO: not implemented
	return nullptr;
}

Track * WorkingSm::GetTrack()
{
	//ifstream* stream = new ifstream(getPathForFile(smInfo->metadata->AudioFile));
	char* temp = (char*)getPathForFile(smInfo->metadata->AudioFile).c_str();

	return new BassTrack(temp);
}

Sm<Event>* WorkingSm::createSm()
{
	Sm<Event>* s;

	// get path for file�O�b�o��sm set�̭���L�̥�����path�A�M���path�M�ɦW�a�b�@�_�A�^��
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

