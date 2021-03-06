#include "FakeMeteoDatabaseContext.h"


using namespace Framework::Database;


vector<SmInfo*>* FakeMeteoDatabaseContext::parseSmInfoCsv(fstream * stream)
{
	vector<SmInfo*>* smInfos = new vector<SmInfo*>();

	string line;

	do {

		getline(*stream, line);

	} while (!stream->eof() && line.empty());

	while (!stream->eof()) {
	
		getline(*stream, line);
		if (line != "") {
			SmInfo* newInfo = new SmInfo();
			newInfo->fileName = line;
			smInfos->push_back(newInfo);
		}

	}

	return smInfos;
}

FakeMeteoDatabaseContext::FakeMeteoDatabaseContext(Storage * s): MeteoDatabaseContext(s)
{
}

int FakeMeteoDatabaseContext::prepare()
{
	if (!storage->Exist("SmInfos.csv")) {
		
		// 還沒寫完 先留著
		return 0;

		throw runtime_error("FakeMeteoDatabaseContext::prepare(): sminfo.csv not found.");
	}
	LOG(LogLevel::Debug) << "FakeMeteoDatabaseContext::prepare() : parsing infos.";
	fstream* stream = storage->GetStream("SmInfos.csv");

	vector<SmInfo*>* smInfos = parseSmInfoCsv(stream);

	for (int i = 0; i < smInfos->size(); i++)
		AddDbSetEntity<SmInfo>(smInfos->at(i));

	LOG(LogLevel::Debug) << "FakeMeteoDatabaseContext::prepare() : adding entity.";
	delete smInfos;
	delete stream;

	LOG(LogLevel::Debug) << "FakeMeteoDatabaseContext::prepare() : end.";
	return 0;
}
