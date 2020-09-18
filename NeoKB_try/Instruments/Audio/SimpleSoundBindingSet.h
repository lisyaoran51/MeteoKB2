#ifndef SIMPLE_SOUND_BINDING_SET_H
#define SIMPLE_SOUND_BINDING_SET_H




#include <string>
#include "../Pitch.h"
#include "SoundBindingSet.h"
#include "SimpleSoundBinding.h"

using namespace std;
using namespace Instruments;

namespace Instruments {
namespace Audio {

	class SimpleSoundBindingSet : public TSoundBindingSet<Pitch> {

	public:

		SimpleSoundBindingSet(string fName);

		virtual string GetFileName(Pitch p);

		virtual SimpleSoundBinding<Pitch>* GetSoundBinding(Pitch p);


	protected:


	};

}}









#endif