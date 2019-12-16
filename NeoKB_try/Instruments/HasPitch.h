#ifndef HAS_PITCH_H
#define HAS_PITCH_H


#include "Pitch.h"


namespace Instruments {

	class HasPitch {

	public:

		HasPitch(Pitch p);

		virtual Pitch GetPitch();

	protected:

		Pitch pitch;

	};

}






#endif
