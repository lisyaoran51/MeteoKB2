#ifndef SOUND_BINDING_H
#define SOUND_BINDING_H

#include <string>


using namespace std;

namespace Instruments {
namespace Audio {

	class SoundBinding {

	public:

		SoundBinding(string sBankName, int a);
		virtual ~SoundBinding() = default;

		string soundBankName;

		int action;

		virtual string GetSoundBankName();

		virtual string GetFileName() = 0;

	};



	template<typename T>
	class TSoundBinding : public SoundBinding {

	public:

		TSoundBinding(string sBankName, T a) : SoundBinding(sBankName, (int)a){
		}

		T GetAction() {
			return dynamic_cast<T>(action);
		}
		
		virtual string GetFileName() {
			return soundBankName;
		}

	};

}}




#endif