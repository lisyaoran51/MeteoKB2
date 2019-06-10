#ifndef SM_DECODER_H
#define SM_DECODER_H

#include <string>
#include <fstream>
#include <map>
#include <stdexcept>
#include "../../../Util/MtoObject.h"
#include "../Sheetmusic.h"
#include "../../Scheduler/Event/Event.h"


using namespace std;
using namespace Util;
using namespace Base::Sheetmusics;
using namespace Base::Schedulers::Events;

// inlcude path defined by macro
// https://stackoverflow.com/questions/32066204/construct-path-for-include-directive-with-macro
//


namespace Base {
namespace Sheetmusics {
namespace Format {


	class SmDecoder: public MtoObject
	{
		// sm decoder 不是singleton
		// friend class Singleton<SmDecoder>;

		static map<string, string> decoders;

	public:

		static int RegisterDecoder(string version, string typeName);
		static SmDecoder* GetDecoder(ifstream* stream);
		Sm<Event>* Decode(ifstream* stream);

	protected:

		SmDecoder();

		virtual Sm<Event>* parseFile(ifstream* stream);
		virtual int parseFile(ifstream* stream, Sm<Event>* sm) = 0;
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