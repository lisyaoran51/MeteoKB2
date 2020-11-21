#ifndef PITCH_BINDING_SET_H
#define PITCH_BINDING_SET_H

#include <string>
#include "../Pitch.h"
#include <map>


using namespace std;
using namespace Instruments;


namespace Instruments {
namespace Input {

	struct PitchBindingSet {

	public:

	};

	template<typename T>
	struct TPitchBindingSet : public PitchBindingSet {

		

	public:

		virtual int SetPitchBinding(Pitch pitch, T action) = 0;

		virtual T GetAction(Pitch pitch) = 0;

		virtual Pitch GetPitch(T action) = 0;

	protected:

		map<T, Pitch> actionToPitch;
		map<Pitch, T> pitchToAction;


	};
}}



#endif