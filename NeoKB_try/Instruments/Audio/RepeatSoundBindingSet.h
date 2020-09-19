#ifndef REPEAT_SOUND_BINDING_SET_H
#define REPEAT_SOUND_BINDING_SET_H




#include <string>
#include "../Pitch.h"
#include "SoundBindingSet.h"
#include "RepeatSoundBinding.h"

using namespace std;
using namespace Instruments;

namespace Instruments {
namespace Audio {

	class RepeatSoundBindingSet : public TSoundBindingSet<Pitch> {

	public:

		RepeatSoundBindingSet();
		RepeatSoundBindingSet(string fName, float tLength);

		float tailLength = 10.f;

		virtual string GetFileName(Pitch p);

		virtual RepeatSoundBinding<Pitch>* GetSoundBinding(Pitch p);



	};

}}









#endif