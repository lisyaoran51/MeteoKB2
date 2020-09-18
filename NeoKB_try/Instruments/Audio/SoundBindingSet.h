#ifndef SOUND_BINDING_SET_H
#define SOUND_BINDING_SET_H




#include <string>
#include "../Pitch.h"
#include "SoundBinding.h"


using namespace std;
using namespace Instruments;


namespace Instruments {
namespace Audio {

	class SoundBindingSet {

	public:
		SoundBindingSet(string fName);

	protected:

		string fileName;

	};

	template<typename T>
	class TSoundBindingSet : public SoundBindingSet {

	public:

		TSoundBindingSet(string fName): SoundBindingSet(fName){}

		int SetKeySpan(T sKey, T eKey) {
			startKey = sKey;
			endKey = eKey;
			return 0;
		}

		T GetStartKey() { return startKey; }
		T GetEndKey() { return endKey; }

		virtual string GetFileName(T p) = 0;

		virtual TSoundBinding<T>* GetSoundBinding(T p) = 0;

	protected:

		T startKey;
		T endKey;

	};


}}









#endif