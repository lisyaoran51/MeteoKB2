#ifndef SOUND_BINDING_SET_DECODER_H
#define SOUND_BINDING_SET_DECODER_H

#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include "../../../Allocation/Hierachal/MtoObject.h"
#include "../../../../Instruments/Audio/SoundBindingSet.h"


using namespace std;
using namespace Framework::Allocation::Hierachal;
using namespace Instruments::Audio;


// inlcude path defined by macro
// https://stackoverflow.com/questions/32066204/construct-path-for-include-directive-with-macro
//


namespace Framework {
namespace Audio {
namespace Samples {
namespace Format {


	class SoundBindingSetDecoder: public MtoObject
	{
		// sm decoder ���Osingleton
		// friend class Singleton<SmDecoder>;

		static bool initialized;

		static map<string, string> decoders;

	public:

		static int Initialize();

		static int RegisterDecoder(string version, string typeName);
		static SoundBindingSetDecoder* GetDecoder(fstream* stream);
		SoundBindingSet* Decode(fstream* stream);

	protected:

		SoundBindingSetDecoder();

		virtual SoundBindingSet* parseFile(fstream* stream);
		virtual int parseFile(fstream* stream, SoundBindingSet* sm) = 0;
	};

	

	template<typename T>
	class SoundBindingSetDecoderWithSection : public SoundBindingSetDecoder {

	protected:

		SoundBindingSetDecoderWithSection() : SoundBindingSetDecoder(), RegisterType("SoundBindingSetDecoderWithSection")
		{
		}

		// �]��c++���䴩enum������string�A�ҥH�n�ۤv�g
		map<T, string> sectionMap;

		virtual int setSectionMap() = 0;

		T GetSectionEnum(string section) {
			// �p�G�٨S���U�A�����U
			if (sectionMap.size() == 0)
				setSectionMap();

			typename map<T, string>::iterator i;

			for (i = sectionMap.begin(); i != sectionMap.end(); i++)
			{
				if (i->second == section)
					return i->first;
			}
			return static_cast<T>(0);
		}

		string GetSectionString(T section) {
			// �p�G�٨S���U�A�����U
			if (sectionMap.size() == 0)
				setSectionMap();

			typename map<T, string>::iterator i = sectionMap.find(section);

			if (i != sectionMap.end())
				return sectionMap[section];
			else
				return "";
		}

	};

}}}}



#endif