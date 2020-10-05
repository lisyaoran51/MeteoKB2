#include "SimpleSoundBindingSetDecoder.h"

#include "../../../Util/Log.h"
#include "../SimpleSoundBindingSet.h"
#include "../RepeatSoundBindingSet.h"
#include "../TwoStageSoundBindingSet.h"


using namespace Instruments::Audio::Format;
using namespace std;
using namespace Util;
using namespace Instruments::Audio;


/*
 * 切字串的方法
 * https://blog.csdn.net/Vincentlmeng/article/details/73201188
 * https://www.cnblogs.com/MikeZhang/archive/2012/03/24/MySplitFunCPP.html
 */




int SimpleSoundBindingSetDecoder::handleGeneral(SoundBindingSet* sbs, string & line)
{
	vector<string> pair = split(line, ":");

	LOG(LogLevel::Finer) << "int SimpleSoundBindingSetDecoder::handleGeneral() : tag is[" << pair.at(0) << "]. value is [" << pair.at(1) << "].";


	if (pair.at(0) == "Mode") {
		switch (atoi(pair.at(1).c_str())) {
		case 1:
			sbs = new SimpleSoundBindingSet();
			break;
		case 2:
			sbs = new RepeatSoundBindingSet();
			break;
		case 3:
			sbs = new TwoStageSoundBindingSet();
			break;
		}
	}
	else if (pair.at(0) == "Filename") {
		sbs->fileName = pair.at(1);
		LOG(LogLevel::Finer) << "int SimpleSoundBindingSetDecoder::handleGeneral() : tag [Filename] chosen.";
	}
	else if (pair.at(0) == "Start") {
		dynamic_cast<TSoundBindingSet<Pitch>*>(sbs)->startKey = (Pitch)atoi(pair.at(1).c_str());
	}
	else if (pair.at(0) == "End") {
		dynamic_cast<TSoundBindingSet<Pitch>*>(sbs)->endKey = (Pitch)atoi(pair.at(1).c_str());
	}
	else if (pair.at(0) == "SwitchPoint") {
		if (dynamic_cast<TwoStageSoundBindingSet*>(sbs)) {
			dynamic_cast<TwoStageSoundBindingSet*>(sbs)->switchPoint = atof(pair.at(1).c_str());
		}
		else {
			LOG(LogLevel::Error) << "int SimpleSoundBindingSetDecoder::handleGeneral() : sound binding set type is not TwoStageSoundBindingSet.";
		}
	}
	else if (pair.at(0) == "TailLength") {
		if (dynamic_cast<RepeatSoundBindingSet*>(sbs)) {
			dynamic_cast<RepeatSoundBindingSet*>(sbs)->tailLength = atof(pair.at(1).c_str());
		}
		else if (dynamic_cast<TwoStageSoundBindingSet*>(sbs)) {
			dynamic_cast<TwoStageSoundBindingSet*>(sbs)->tailLength = atof(pair.at(1).c_str());
		}
		else {
			LOG(LogLevel::Error) << "int SimpleSoundBindingSetDecoder::handleGeneral() : sound binding set type has no tail length.";
		}
	}

	return 0;
}

vector<string> SimpleSoundBindingSetDecoder::split(string s, string pattern)
{
	string::size_type pos;
	vector<string> result;
	s += pattern;//扩展字符串以方便操作
	int size = s.size();

	for (int i = 0; i < size; i++)
	{
		pos = s.find(pattern, i);
		if (pos < size)
		{
			string split = s.substr(i, pos - i);
			result.push_back(split);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

SimpleSoundBindingSetDecoder::SimpleSoundBindingSetDecoder() : SoundBindingSetDecoderWithSection(), RegisterType("SimpleSoundBindingSetDecoder")
{
	setSectionMap();
}

int SimpleSoundBindingSetDecoder::setSectionMap()
{

	sectionMap[SimpleSoundBindingSetDecoderSection::None] = "None";
	sectionMap[SimpleSoundBindingSetDecoderSection::General] = "General";
	sectionMap[SimpleSoundBindingSetDecoderSection::Editor] = "Editor";
	sectionMap[SimpleSoundBindingSetDecoderSection::Metadata] = "Metadata";
	sectionMap[SimpleSoundBindingSetDecoderSection::Variables] = "Variables";
	return 0;

}

int SimpleSoundBindingSetDecoder::parseFile(fstream * stream, SoundBindingSet* sbs)
{
	LOG(LogLevel::Fine) << "int SimpleSoundBindingSetDecoder::parseFile() : Start parsing file.";

	string line;

	do {

		try
		{
			getline(*stream, line);
		}
		catch (const exception& e) //it would not work if you pass by value
		{
			LOG(LogLevel::Error) << "int SimpleSoundBindingSetDecoder::parseFile() : " << e.what();
		}
		LOG(LogLevel::Finer) << "int SimpleSoundBindingSetDecoder::parseFile() : read line [" << line << "].";

	} while (!stream->eof() && line.empty());

	//sheetmusic.SheetmusicInfo.SheetmusicVersion = sheetmusicVersion;
	// rfind可能有問題，他可能是從後面算
	//sm->GetSmInfo()->smVersion = atoi(line.substr(line.rfind("v"), line.size() - line.rfind("v")).c_str());



	SimpleSoundBindingSetDecoderSection section = SimpleSoundBindingSetDecoderSection::None;
	//bool hasCustomColours = false;


	while (!stream->eof()) {
		getline(*stream, line);

		LOG(LogLevel::Finer) << "int SimpleSmDecoder::parseFile(ifstream*, Sm<Event>*) : read line [" << line << "].";

		if (line == " " || line.empty())
			continue;

		if (line.find("//") == 0)
			continue;

		if (line.find("simple file format v") == 0) {
			// 這個寫法好巷不general?
			// sm->GetSmInfo()->smVersion = atoi(line.substr(20, line.size() - 20).c_str());
			continue;
		}

		if (line.find("[") == 0 && line.rfind("]") == line.size() - 1) {

			section = GetSectionEnum(line.substr(1, line.size() - 2));
			LOG(LogLevel::Finer) << "int SimpleSoundBindingSetDecoder::parseFile() : set section [" << line.substr(1, line.size() - 2) << "].";
			continue;
		}

		switch (section) {
		case SimpleSoundBindingSetDecoderSection::General:
			handleGeneral(sbs, line);
			break;
		case SimpleSoundBindingSetDecoderSection::Metadata:
			//handleMetadata(sm, line);
			break;
			
		}
	}
	LOG(LogLevel::Fine) << "int SimpleSoundBindingSetDecoder::parseFile() : parsing file over.";
	return 0;
}
