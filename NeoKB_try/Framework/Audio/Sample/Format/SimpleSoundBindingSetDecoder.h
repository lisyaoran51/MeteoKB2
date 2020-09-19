#ifndef SIMPLE_SOUND_BINDING_SET_DECODER_H
#define SIMPLE_SOUND_BINDING_SET_DECODER_H

#include "SoundBindingSetDecoder.h"
#include <vector>


using namespace std;



namespace Framework {
namespace Audio {
namespace Samples {
namespace Format {

	enum class SimpleSoundBindingSetDecoderSection;

	/// <summary>
	/// simple�����O�u�N�@�릱�Ц����ǭ��Эp�X�ӡA���|�hŪ�o���и̭��n�B�~�[�J����S�ĩΰʵe
	/// </summary>
	class SimpleSoundBindingSetDecoder : public SoundBindingSetDecoderWithSection<SimpleSoundBindingSetDecoderSection>
	{

		int handleGeneral(SoundBindingSet* sbs, string& line);
	
		vector<string> split(string s, string pattern);

	public:

		SimpleSoundBindingSetDecoder();

	protected:

		virtual int setSectionMap();

		virtual int parseFile(fstream* stream, SoundBindingSet* sm);
	};

	enum class SimpleSoundBindingSetDecoderSection {
		None,
		General,
		Editor,
		Metadata,
		Variables,
	};

}}}}





#endif
