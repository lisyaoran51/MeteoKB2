#ifndef TWO_STAGE_SOUND_BINDING_H
#define TWO_STAGE_SOUND_BINDING_H


#include "SoundBinding.h"


namespace Instruments {
namespace Audio {
	
	template<typename T>
	class TwoStageSoundBinding : public TSoundBinding<T> {

	public:
		TwoStageSoundBinding(string sBankName, T a, float sPoint, float tLength) : TSoundBinding<T>(sBankName, a) {
			switchPoint = sPoint;
			tailLength = tLength;
		}

		float switchPoint = 5.f;
		float tailLength = 10.f;

		string GetStageAFileName() {
			return SoundBinding::soundBankName + string("_A_") + to_string(int(SoundBinding::action));
		}

		string GetStageBFileName() {
			return SoundBinding::soundBankName + string("_B_") + to_string(int(SoundBinding::action));
		}

	};

}}



#endif