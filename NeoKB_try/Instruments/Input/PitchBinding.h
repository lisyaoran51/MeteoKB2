#ifndef PITCH_BINDING_H
#define PITCH_BINDING_H

#include <string>
#include "../../Framework/Input/KeyBindings/InputKey.h"
#include "../Pitch.h"


using namespace std;
using namespace Framework::Input::KeyBindings;
using namespace Instruments;


namespace Instruments {
namespace Input {

	//¼g¿ù¤F
	/*
	struct PitchBinding {

		int action ;
		Pitch pitch;

		PitchBinding(int a, Pitch p) {
			action = a;
			pitch = p;
		}

		template<typename T>
		T GetAction() {
			return (T)action;// dynamic_cast<T>(action);
		}

		string ToString() {
			return to_string(action);
		}



	};

	template<typename T>
	struct TPitchBinding : public PitchBinding {

		TPitchBinding(T a, Pitch p) : PitchBinding((int)a, p) {
		}

		T GetAction() {
			return dynamic_cast<T>(action);
		}


	};
	*/
}}



#endif