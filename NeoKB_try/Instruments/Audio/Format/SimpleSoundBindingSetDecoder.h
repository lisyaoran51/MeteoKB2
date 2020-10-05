#ifndef SIMPLE_SOUND_BINDING_SET_DECODER_H
#define SIMPLE_SOUND_BINDING_SET_DECODER_H

#include "SoundBindingSetDecoder.h"
#include <vector>


using namespace std;



namespace Instruments {
namespace Audio {
namespace Format {

	enum class SimpleSoundBindingSetDecoderSection;

	/// <summary>
	/// simple易死是只將一般曲譜有哪些音標計出來，不會去讀這個譜裡面要額外加入什麼特效或動畫
	/// </summary>
	class SimpleSoundBindingSetDecoder : public SoundBindingSetDecoderWithSection<SimpleSoundBindingSetDecoderSection>
	{
		SoundBindingSet* handleMode(string& line);

		int handleGeneral(SoundBindingSet* sbs, string& line);
	
		vector<string> split(string s, string pattern);

	public:

		SimpleSoundBindingSetDecoder();

	protected:

		virtual int setSectionMap();

		virtual SoundBindingSet* parseFile(fstream* stream);
	};

	enum class SimpleSoundBindingSetDecoderSection {
		None,
		Mode,
		General,
		Editor,
		Metadata,
		Variables,
	};

}}}





#endif
