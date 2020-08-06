#ifndef SM_DECODER_H
#define SM_DECODER_H

#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include "../../../Framework/Allocation/Hierachal/MtoObject.h"
#include "../Sheetmusic.h"
#include "../../Scheduler/Event/Event.h"


using namespace std;
using namespace Framework::Allocation::Hierachal;
using namespace Games::Sheetmusics;
using namespace Games::Schedulers::Events;

// inlcude path defined by macro
// https://stackoverflow.com/questions/32066204/construct-path-for-include-directive-with-macro
//


namespace Games {
namespace Sheetmusics {
namespace Format {


	class SmDecoder: public MtoObject
	{
		// sm decoder 不是singleton
		// friend class Singleton<SmDecoder>;

		static bool initialized;

		static map<string, string> decoders;

	public:

		static int Initialize();

		static int RegisterDecoder(string version, string typeName);
		static SmDecoder* GetDecoder(fstream* stream);
		Sm<Event>* Decode(fstream* stream);

	protected:

		SmDecoder();

		virtual Sm<Event>* parseFile(fstream* stream);
		virtual int parseFile(fstream* stream, Sm<Event>* sm) = 0;
	};

	

	template<typename T>
	class SmDecoderWithSection : public SmDecoder {

	protected:

		SmDecoderWithSection() : SmDecoder(), RegisterType("SmDecoderWithSection")
		{
		}

		// 因為c++不支援enum直接轉string，所以要自己寫
		map<T, string> sectionMap;

		virtual int setSectionMap() = 0;

		T GetSectionEnum(string section) {
			// 如果還沒註冊，先註冊
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
			// 如果還沒註冊，先註冊
			if (sectionMap.size() == 0)
				setSectionMap();

			typename map<T, string>::iterator i = sectionMap.find(section);

			if (i != sectionMap.end())
				return sectionMap[section];
			else
				return "";
		}

	};

}}}



#endif