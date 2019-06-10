#ifndef SIMPLE_SHEETMUSIC_DECODER_H
#define SIMPLE_SHEETMUSIC_DECODER_H

#include "SmDecoder.h"


using namespace std;
using namespace Util;
using namespace Base::Sheetmusics;
using namespace Base::Schedulers::Events;



namespace Base {
namespace Sheetmusics {
namespace Format {

	enum class SimpleSmDecoderSection;

	/// <summary>
	/// simple易死是只將一般曲譜有哪些音標計出來，不會去讀這個譜裡面要額外加入什麼特效或動畫
	/// </summary>
	class SimpleSmDecoder: public SmDecoderWithSection<SimpleSmDecoderSection>
	{

		int defaultSampleVolume = 100;

		int handleGeneral(Sm<Event>* sm, string& line);
		int handleMetadata(Sm<Event>* sm, string& line);
		int handleDifficulty(Sm<Event>* sm, string& line);
		int handleNoteControlPoints(Sm<Event>* sm, string& line);
	
		vector<string> split(string s, string pattern);

	public:

		SimpleSmDecoder();

	protected:

		virtual int setSectionMap();

		virtual int parseFile(ifstream* stream, Sm<Event>* sm);
	};

	enum class SimpleSmDecoderSection {
		None,
		General,
		Editor,
		Metadata,
		Difficulty,
		Events,
		NoteControlPoints,
		Variables,
	};

}}}





#endif
