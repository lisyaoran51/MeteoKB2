#ifndef SOUND_BINDING_H
#define SOUND_BINDING_H

#include <string>

using namespace std;

namespace Instruments {
namespace Audio {

	struct SoundBinding {

		SoundBinding(string fName, int a) {
			fileName = fName;
			action = a;
		}

		string fileName;

		int action;

	};



	template<typename T>
	struct TSoundBinding : public SoundBinding {

		TSoundBinding(string fName, T a) {
			fileName = fName;

			action = (int)a;
		}

		T GetAction() {
			return dynamic_cast<T>(action);
		}
		

	};

}}




#endif