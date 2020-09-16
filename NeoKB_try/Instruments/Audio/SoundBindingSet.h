#ifndef SOUND_BINDING_SET_H
#define SOUND_BINDING_SET_H




#include <string>
#include "../Pitch.h"

using namespace std;
using namespace Instruments;

namespace Instruments {
namespace Audio {

	class SoundBindingSet {

	public:
		SoundBindingSet(string fName);

		int SetKeySpan(Pitch sKey, Pitch eKey);

		Pitch GetStartKey();
		Pitch GetEndKey();

		string GetFileName(Pitch p);

	protected:

		string fileName;

		Pitch startKey = Pitch::A2;
		Pitch EndKey = Pitch::c5;

	};

}}









#endif