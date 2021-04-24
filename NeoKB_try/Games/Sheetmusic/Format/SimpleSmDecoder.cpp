#include "SimpleSmDecoder.h"

#include "../../Scheduler/Event/ControlPoints/NoteControlPoint.h"
#include "../../Scheduler/Event/ControlPoints/InputKeyControlPoint.h"
#include "../../Scheduler/Event/ControlPoints/OctaveAutoControlPoint.h"
#include "../../Scheduler/Event/ControlPoints/SectionStartControlPoint.h"



using namespace Games::Sheetmusics::Format;
using namespace std;
using namespace Games::Sheetmusics;
using namespace Games::Schedulers::Events::ControlPoints;


/*
 * 切字串的方法
 * https://blog.csdn.net/Vincentlmeng/article/details/73201188
 * https://www.cnblogs.com/MikeZhang/archive/2012/03/24/MySplitFunCPP.html
 */




int SimpleSmDecoder::handleGeneral(Sm<Event>* sm, string & line)
{
	vector<string> pair = split(line, ":");

	LOG(LogLevel::Finer) << "int SimpleSmDecoder::handleGeneral(Sm<Event>*,string) : tag is[" << pair.at(0) << "]. value is [" << pair.at(1) << "].";

    SmMetadata* metadata = sm->GetSmInfo()->metadata;

	if (pair.at(0) == "AudioFilename") {
		LOG(LogLevel::Finer) << "int SimpleSmDecoder::handleGeneral(Sm<Event>*,string) : tag [AudioFilename] chosen.";
		metadata->AudioFile = pair.at(1);
		metadata->hasAudioFile = true;
	}
	else if (pair.at(0) == "AudioLeadIn") {
		sm->GetSmInfo()->audioLeadIn = atoi(pair.at(1).c_str());
	}
	else if (pair.at(0) == "PreviewTime") {
		metadata->PreviewTime = atoi(pair.at(1).c_str());
	}
	else if (pair.at(0) == "Countdown") {
		sm->GetSmInfo()->countdown = atoi(pair.at(1).c_str()) == 1;
		//metadata->AudioFile = pair.at(1);
	}
	else if (pair.at(0) == "SampleSet") {
		//metadata->AudioFile = pair.at(1);
	}
	else if (pair.at(0) == "SampleVolume") {
		defaultSampleVolume = atoi(pair.at(1).c_str());
	}
	else if (pair.at(0) == "StackLeniency") {
		//metadata->AudioFile = pair.at(1);
	}
	else if (pair.at(0) == "Mode") {
		// Mode 1 : Meteor
		// Mode 2 : Instant
		// Mode 3 : Record
		// Mode 4 : MeteorRecord
		sm->GetSmInfo()->rulesetId = atoi(pair.at(1).c_str());
	}
	else if (pair.at(0) == "LetterboxInBreaks") {
		//metadata->AudioFile = pair.at(1);
	}
	else if (pair.at(0) == "SpecialStyle") {
		//metadata->AudioFile = pair.at(1);
	}
	else if (pair.at(0) == "Pedal") {	/* 代表這份譜有標示踏板 */
		sm->GetSmInfo()->hasPedalData = atoi(pair.at(1).c_str()) == 1;
	}
	else if (pair.at(0) == "Section") {	/* 代表這份譜有標示小節 */
		sm->GetSmInfo()->hasSectionData = atoi(pair.at(1).c_str()) == 1;
	}
	else if (pair.at(0) == "HandType") { 
		switch (atoi(pair.at(1).c_str())) {
		case 0:
			sm->GetSmInfo()->smHandType == SmHandType::None;
			break;
		case 1:
			sm->GetSmInfo()->smHandType == SmHandType::Hand;
			break;
		case 2:
			sm->GetSmInfo()->smHandType == SmHandType::Difficulty;
			break;
		case 3:
			sm->GetSmInfo()->smHandType == SmHandType::All;
			break;

		}
	}
    
	return 0;
}

int SimpleSmDecoder::handleMetadata(Sm<Event>* sm, string & line)
{
	vector<string> pair = split(line, ":");

	SmMetadata* metadata = sm->GetSmInfo()->metadata;

	if (pair.at(0) == "Title") {
		metadata->Title = pair.at(1);
	}
	else if (pair.at(0) == "TitleUnicode") {
		//sm->GetSmInfo()->audioLeadIn = atoi(pair.at(1).c_str());
	}
	else if (pair.at(0) == "Artist") {
		metadata->Artist = pair.at(1);
	}
	else if (pair.at(0) == "ArtistUnicode") {
		//metadata->AudioFile = pair.at(1);
	}
	else if (pair.at(0) == "AudioFile") {
		metadata->AudioFile = pair.at(1);
	}
	else if (pair.at(0) == "Creator") {
		metadata->Author = pair.at(1);
	}
	else if (pair.at(0) == "Version") {
		sm->GetSmInfo()->version = atoi(pair.at(1).c_str());
	}
	else if (pair.at(0) == "Source") {
		metadata->Source = pair.at(1);
	}
	else if (pair.at(0) == "Tags") {
	}
	else if (pair.at(0) == "SheetmusicID") {
		//metadata->AudioFile = pair.at(1);
	}
	else if (pair.at(0) == "SheetmusicSetID") {
		//metadata->AudioFile = pair.at(1);
	}


	return 0;
}

int SimpleSmDecoder::handleDifficulty(Sm<Event>* sm, string & line)
{
	vector<string> pair = split(line, ":");

	SmDifficulty* difficulty = sm->GetSmInfo()->difficuty;
	
	if (pair.at(0) == "Speed") {
		difficulty->Speed = atoi(pair.at(1).c_str());
	}

	/*
	switch (pair.Key) {
        case @"HPDrainRate":
            difficulty.DrainRate = float.Parse(pair.Value);
            break;
        case @"OverallDifficulty":
            difficulty.OverallDifficulty = float.Parse(pair.Value);
            break;
        case @"ApproachRate":
            difficulty.ApproachRate = float.Parse(pair.Value);
            break;
        case @"SliderMultiplier":
            difficulty.SliderMultiplier = float.Parse(pair.Value);
            break;
        case @"SliderTickRate":
            difficulty.SliderTickRate = float.Parse(pair.Value);
            break;
    }
	*/

			

	return 0;
}

int SimpleSmDecoder::handleNoteControlPoints(Sm<Event>* sm, string & line)
{
	// 音 時間 長度 加速 三分/四分 音量 時間改變
	vector<string> splitLine = split(line, ",");

	int pitchInt = atoi(splitLine.at(0).c_str());

	MTO_FLOAT time = stof(splitLine.at(1).c_str());
	MTO_FLOAT noteLength = stof(splitLine.at(2).c_str());
	
	MTO_FLOAT volume 	= splitLine.size() > 3 ? stof(splitLine.at(3).c_str()) : -1;
	int sectionIndex 	= splitLine.size() > 4 ? atoi(splitLine.at(4).c_str()) : -1;
	int hand 			= splitLine.size() > 5 ? atoi(splitLine.at(5).c_str()) : 0;
	int partIndex		= splitLine.size() > 6 ? atoi(splitLine.at(6).c_str()) : -1;
	
	//MTO_FLOAT speedMultiplier = noteLength < 0 ? 100f / -noteLength : 1;
	//
	//TimeSignatures timeSignature = TimeSignatures.SimpleQuadruple;
	//if (split.Length >= 4)
	//	timeSignature = split[3][0] == '0' ? TimeSignatures.SimpleQuadruple : (TimeSignatures)int.Parse(split[2]);

	//LegacySampleBank sampleSet = defaultSampleBank;
	//if (split.Length >= 4)
	//    sampleSet = (LegacySampleBank)int.Parse(split[3]);

	//SampleBank sampleBank = SampleBank.Default;
	//if (split.Length >= 5)
	//    sampleBank = (SampleBank)int.Parse(split[4]);

	//int sampleVolume = defaultSampleVolume;
	//if (splitLine.size() >= 5)
	//	sampleVolume = atoi(splitLine.at(4).c_str());
    //
	bool timingChange = true;
	//if (splitLine.size() >= 6)
	//	timingChange = atoi(splitLine.at(5).c_str()) == 1;

	//bool kiaiMode = false;
	//bool omitFirstBarSignature = false;
	//if (split.Length >= 8) {
	//    int effectFlags = int.Parse(split[7]);
	//    kiaiMode = (effectFlags & 1) > 0;
	//    omitFirstBarSignature = (effectFlags & 8) > 0;
	//}

	//string stringSampleSet = sampleSet.ToString().ToLower();
	//if (stringSampleSet == @"none")
	//    stringSampleSet = @"normal";

	//DifficultyControlPoint difficultyPoint = beatmap.ControlPointInfo.DifficultyPointAt(time);
	//SoundControlPoint soundPoint = beatmap.ControlPointInfo.SoundPointAt(time);
	//EffectControlPoint effectPoint = beatmap.ControlPointInfo.EffectPointAt(time);

	if (timingChange) {

		MarkControlPoint* newMarkControlPoint = nullptr;

		if (pitchInt >= 0) {
			Pitch pitch = static_cast<Pitch>(pitchInt);

			newMarkControlPoint = new NoteControlPoint(
				pitch,
				time,
				noteLength >= 0 ? noteLength : 0
			);

			if (volume <= 0 || volume > 127)
				volume = 50.f;// defaultSampleVolume;

		}
		else {
			InputKey inputKey = InputKey::None;
			int inputValue = 0;
			switch (pitchInt)
			{
			case -1:
				inputKey = InputKey::SustainPedal;
				newMarkControlPoint = new InputKeyControlPoint(inputKey, inputValue, time, noteLength >= 0 ? noteLength : 0);
				break;
			case -2:
				inputKey = InputKey::LowerOctave; // 這邊程式邏輯有問題，遊戲中升降8度是不能夠手動控制的，只能電腦自動控制
												  // 應該要放在Event section裡面
				newMarkControlPoint = new InputKeyControlPoint(inputKey, inputValue, time, noteLength >= 0 ? noteLength : 0);
				break;
			case -3:
				inputKey = InputKey::RaiseOctave; // 這邊程式邏輯有問題，遊戲中升降8度是不能夠手動控制的，只能電腦自動控制
												  // 應該要放在Event section裡面
				newMarkControlPoint = new InputKeyControlPoint(inputKey, inputValue, time, noteLength >= 0 ? noteLength : 0);
				break;
			case -4:
				newMarkControlPoint = new SectionStartControlPoint(sectionIndex, time, noteLength >= 0 ? noteLength : 0);
				LOG(LogLevel::Depricated) << "int SimpleSmDecoder::handleNoteControlPoints() : Section [" << sectionIndex << "] at [" << time << "].";
				break;
			}
			
		}

		newMarkControlPoint->SetSectionIndex(sectionIndex);
		newMarkControlPoint->SetPartIndex(partIndex);

		/* 
		 * 如果這個控制點是音符，就加入[音量]
		 * 如果控制點是可彈奏控制點，就加入[使用手]
		 */
		if (dynamic_cast<PlayableControlPoint*>(newMarkControlPoint)) {
			dynamic_cast<PlayableControlPoint*>(newMarkControlPoint)->SetHandType(static_cast<HandType>(hand));
			if (dynamic_cast<NoteControlPoint*>(newMarkControlPoint)) {
				dynamic_cast<NoteControlPoint*>(newMarkControlPoint)->SetVolume(float(volume) / 128.f);
			}
		}


		if (pitchInt >= 0)
			LOG(LogLevel::Depricated) << "int SimpleSmDecoder::handleNoteControlPoints() : Note [" << pitchInt << "] at [" << time << "] volume [" << volume / 128.f << "].";


		sm->GetEvents()->push_back(newMarkControlPoint);
	}

	//if (speedMultiplier != difficultyPoint.SpeedMultiplier) {
	//    beatmap.ControlPointInfo.DifficultyPoints.RemoveAll(x => x.Time == time);
	//    beatmap.ControlPointInfo.DifficultyPoints.Add(new DifficultyControlPoint {
	//        Time = time,
	//        SpeedMultiplier = speedMultiplier
	//    });
	//}

	//if (stringSampleSet != soundPoint.SampleBank || sampleVolume != soundPoint.SampleVolume) {
	//    beatmap.ControlPointInfo.SoundPoints.Add(new SoundControlPoint {
	//        Time = time,
	//        SampleBank = stringSampleSet,
	//        SampleVolume = sampleVolume
	//    });
	//}

	//if (kiaiMode != effectPoint.KiaiMode || omitFirstBarSignature != effectPoint.OmitFirstBarLine) {
	//    beatmap.ControlPointInfo.EffectPoints.Add(new EffectControlPoint {
	//        Time = time,
	//        KiaiMode = kiaiMode,
	//        OmitFirstBarLine = omitFirstBarSignature
	//    });
	//}
	return 0;
}

vector<string> SimpleSmDecoder::split(string s, string pattern)
{
	string::size_type pos;
     vector<string> result;
     s+=pattern;//扩展字符串以方便操作
     int size=s.size();
 
     for(int i=0; i<size; i++)
     {
         pos=s.find(pattern,i);
         if(pos<size)
         {
             string split=s.substr(i,pos-i);
             result.push_back(split);
             i=pos+pattern.size()-1;
         }
     }
     return result;
}

SimpleSmDecoder::SimpleSmDecoder(): SmDecoderWithSection(), RegisterType("SimpleSmDecoder")
{
	setSectionMap();
}

int SimpleSmDecoder::setSectionMap()
{

	sectionMap[SimpleSmDecoderSection::None] = "None";
	sectionMap[SimpleSmDecoderSection::General] = "General";
	sectionMap[SimpleSmDecoderSection::Editor] = "Editor";
	sectionMap[SimpleSmDecoderSection::Metadata] = "Metadata";
	sectionMap[SimpleSmDecoderSection::Difficulty] = "Difficulty";
	sectionMap[SimpleSmDecoderSection::Events] = "Events";
	sectionMap[SimpleSmDecoderSection::NoteControlPoints] = "NoteControlPoints";
	sectionMap[SimpleSmDecoderSection::Variables] = "Variables";
	return 0;

}

int SimpleSmDecoder::parseFile(fstream * stream, Sm<Event>* sm)
{
	LOG(LogLevel::Fine) << "int SimpleSmDecoder::parseFile(ifstream*, Sm<Event>*) : Start parsing file.";

	string line;

	do {

		getline(*stream, line);
		LOG(LogLevel::Finer) << "int SimpleSmDecoder::parseFile(ifstream*, Sm<Event>*) : read line [" << line << "].";

	} while (!stream->eof() && line.empty());

	//sheetmusic.SheetmusicInfo.SheetmusicVersion = sheetmusicVersion;
	// rfind可能有問題，他可能是從後面算
	//sm->GetSmInfo()->smVersion = atoi(line.substr(line.rfind("v"), line.size() - line.rfind("v")).c_str());



	SimpleSmDecoderSection section = SimpleSmDecoderSection::None;
    //bool hasCustomColours = false;

	
    while (!stream->eof()) {
		getline(*stream, line);

		LOG(LogLevel::Finer) << "int SimpleSmDecoder::parseFile(ifstream*, Sm<Event>*) : read line [" << line << "].";

        if (line==" " || line.empty())
            continue;

        if (line.find("//") == 0)
            continue;

        if (line.find("simple file format v") == 0) {
			// 這個寫法好巷不general?
			sm->GetSmInfo()->smVersion = atoi(line.substr(20, line.size() - 20).c_str());
            continue;
        }

        if (line.find("[") == 0 && line.rfind("]") == line.size() - 1) {

            section = GetSectionEnum(line.substr(1, line.size()-2));
			LOG(LogLevel::Finer) << "int SimpleSmDecoder::parseFile(ifstream*, Sm<Event>*) : set section [" << line.substr(1, line.size() - 2) << "].";
            continue;
        }

        switch (section) {
		case SimpleSmDecoderSection::General:
                handleGeneral(sm, line);
                break;
		case SimpleSmDecoderSection::Metadata:
                handleMetadata(sm, line);
                break;
		case SimpleSmDecoderSection::Difficulty:
                handleDifficulty(sm, line);
                break;
            //case Section.Events:
            //    handleEvents(sheetmusic, line, ref storyboardSprite, ref timelineGroup);
            //    break;
		case SimpleSmDecoderSection::NoteControlPoints:
				// 這裡應該改成根據rulesetId叫parset出來
				// 之後再改，現在還沒需要(沒有其他遊戲規則的譜面)

                handleNoteControlPoints(sm, line);
                break;
            //case Section.Colours:
            //    handleColours(sheetmusic, line, ref hasCustomColours);
            //    break;
		//case SimpleSmDecoderSection::HitObjects:

                // If the ruleset wasn't specified, assume the osu!standard ruleset.
                //if (parser == null)
                //    parser = new Rulesets.Objects.Parsers.ConvertHitObjectParser();
				//
                //var obj = parser.Parse(line);
				//
                //if (obj != null)
                //    sheetmusic.HitObjects.Add(obj);
				//
                //break;
            //case Section.Variables:
            //    handleVariables(line);
            //    break;
        }
    }

	// 不確定下面這行的用途，之後應該要回去osu看
    //foreach (var hitObject in sheetmusic.HitObjects)
    //    hitObject.ApplyDefaults(sheetmusic.ControlPointInfo, sheetmusic.SheetmusicInfo.BaseDifficulty);

	// TODO: 先亂寫，1就是MeteorRuleset
	if (sm->GetSmInfo()->rulesetId == 0)
		sm->GetSmInfo()->rulesetId = 1;

	return 0;
}
