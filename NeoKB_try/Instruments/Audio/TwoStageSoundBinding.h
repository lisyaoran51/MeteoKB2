#ifndef TWO_STAGE_SOUND_BINDING_H
#define TWO_STAGE_SOUND_BINDING_H


#include "SoundBinding.h"


namespace Instruments {
namespace Audio {
	
	template<typename T>
	class TwoStageSoundBinding : public TSoundBinding<T> {

	public:
		TwoStageSoundBinding(string fName, T a, float sPoint, float tLength) : TSoundBinding<T>(fName, a) {
			switchPoint = sPoint;
			tailLength = tLength;
		}

		float GetSwitchPoint() {
			return switchPoint;
		}

		float GetTailLength() {
			return tailLength;
		}

		string GetStageAFileName() {
			return fileName + string("_A_") + to_string(int(action));
		}

		string GetStageBFileName() {
			return fileName + string("_B_") + to_string(int(action));
		}


	protected:

		float switchPoint = 5.f;
		float tailLength = 10.f;

	};

}}



#endif