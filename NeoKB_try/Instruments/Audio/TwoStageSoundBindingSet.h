#ifndef TWO_STAGE_SOUND_BINDING_SET_H
#define TWO_STAGE_SOUND_BINDING_SET_H




#include <string>
#include "../Pitch.h"
#include "SoundBindingSet.h"
#include "TwoStageSoundBinding.h"


using namespace std;
using namespace Instruments;

namespace Instruments {
namespace Audio {

	class TwoStageSoundBindingSet : public TSoundBindingSet<Pitch> {

	public:

		TwoStageSoundBindingSet();
		TwoStageSoundBindingSet(string fName, float sPoint, float tLength);


		float switchPoint = 5.f;
		float tailLength = 10.f;
		
		virtual string GetFileName(Pitch p);

		virtual TwoStageSoundBinding<Pitch>* GetSoundBinding(Pitch p);

		string GetStageAFileName(Pitch p);
		string GetStageBFileName(Pitch p);


	protected:


	};

}}









#endif