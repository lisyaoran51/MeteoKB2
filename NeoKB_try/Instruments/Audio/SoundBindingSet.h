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

		SoundBindingSet();
		SoundBindingSet(string fName);

		virtual ~SoundBindingSet() = default;

		string fileName = "";

	};

	template<typename T>
	class TSoundBindingSet : public SoundBindingSet {

	public:

		TSoundBindingSet(){}
		TSoundBindingSet(string fName): SoundBindingSet(fName){}

		virtual ~TSoundBindingSet() = default;

		T startKey;
		T endKey;

		virtual string GetFileName(T p) = 0;

		virtual TSoundBinding<T>* GetSoundBinding(T p) = 0;


	};


}}









#endif