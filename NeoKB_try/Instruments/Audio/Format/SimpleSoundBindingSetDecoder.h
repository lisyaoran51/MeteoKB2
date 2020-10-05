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
	/// simple�����O�u�N�@�릱�Ц����ǭ��Эp�X�ӡA���|�hŪ�o���и̭��n�B�~�[�J����S�ĩΰʵe
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
