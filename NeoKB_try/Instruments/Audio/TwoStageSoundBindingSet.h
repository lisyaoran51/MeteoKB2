#ifndef TWO_STAGE_SOUND_BINDING_SET_H
#define TWO_STAGE_SOUND_BINDING_SET_H




#include <string>
#include "../Pitch.h"
#include "SoundBindingSet.h"

using namespace std;
using namespace Instruments;

namespace Instruments {
namespace Audio {

	class TwoStageSoundBindingSet : public SoundBindingSet {

	public:
		TwoStageSoundBindingSet(string fName, float sPoint, float tLength);
		
		float GetSwitchPoint();
		float GetTailLength();

		string GetStageAFileName(Pitch p);
		string GetStageBFileName(Pitch p);


	protected:

		float switchPoint = 5.f;
		float tailLength = 10.f;


	};

}}









#endif