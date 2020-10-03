#ifndef SOUND_BINDING_H
#define SOUND_BINDING_H

#include <string>


using namespace std;

namespace Instruments {
namespace Audio {

	class SoundBinding {

	public:

		SoundBinding(string fName, int a);
		virtual ~SoundBinding() = default;

		string fileName;

		int action;

		virtual string GetFileName() = 0;

	};



	template<typename T>
	class TSoundBinding : public SoundBinding {

	public:

		TSoundBinding(string fName, T a) : SoundBinding(fName, (int)a){
			fileName = fName;

			action = (int)a;
		}

		T GetAction() {
			return dynamic_cast<T>(action);
		}
		
		virtual string GetFileName() {
			return fileName;
		}

	};

}}




#endif