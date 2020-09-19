#include "SoundBindingSetDecoder.h"
#include "../../../../Util/InstanceCreator.h"


using namespace std;
using namespace Framework::Audio::Samples::Format;
using namespace Util;

/*
* https://openhome.cc/Gossip/CppGossip/StandardException.html
* error handling in c++
*
*/


map<string, string> SoundBindingSetDecoder::decoders;
bool SoundBindingSetDecoder::initialized = false;

SoundBindingSetDecoder::SoundBindingSetDecoder(): RegisterType("SoundBindingSetDecoder")
{
}

int SoundBindingSetDecoder::Initialize()
{
	SoundBindingSetDecoder::RegisterDecoder("simple file format v0", "SimpleSoundBindingSetDecoder");
	initialized = true;
	return 0;
}

int SoundBindingSetDecoder::RegisterDecoder(string version, string typeName)
{
	LOG(LogLevel::Fine) << "int SoundBindingSetDecoder::RegisterDecoder(string,string) : Registering SoundBindingSet decoder: [" << typeName << "] to version [" << version << "].";
	decoders[version] = typeName;
	return 0;
}

SoundBindingSetDecoder* SoundBindingSetDecoder::GetDecoder(fstream * stream)
{
	if (!initialized)
		Initialize();
	
	string line;

	do {

		getline(*stream, line);

		LOG(LogLevel::Finer) << "SoundBindingSetDecoder* SoundBindingSetDecoder::GetDecoder(ifstream*) : read sm file [" << line << "].";

	} while (!stream->eof() && line.empty());

	stream->seekg(0, ios::beg);

	if (!decoders.count(line)) {
		throw invalid_argument("SoundBindingSetDecoder::GetDecoder()  :input stream has no decoder version in first line.");
	}
	LOG(LogLevel::Fine) << "SoundBindingSetDecoder* SoundBindingSetDecoder::GetDecoder(ifstream*) : Get decoder [" << decoders[line] << "].";

	InstanceCreator<MtoObject> &instance_creater =
		InstanceCreator<MtoObject>::GetInstance();

	return instance_creater.CreateInstanceWithT<SoundBindingSetDecoder>(decoders[line]);
}

SoundBindingSet* SoundBindingSetDecoder::Decode(fstream * stream)
{
	return parseFile(stream);
}

SoundBindingSet* SoundBindingSetDecoder::parseFile(fstream * stream)
{
	SoundBindingSet* sbs = nullptr;

	parseFile(stream, sbs);

	return sbs;
}



