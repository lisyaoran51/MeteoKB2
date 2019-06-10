#ifndef METEOR_PLAYFIELD_H
#define METEOR_PLAYFIELD_H

#include "../../Base/Play/Playfield.h"
#include "../../Util/MtoType.h"
#include "../Config/MeteorConfigManager.h"

using namespace Util;
using namespace Base::Play;
using namespace Base::Schedulers::Events;
using namespace Meteor::Config;



namespace Meteor {
namespace Play{

	class MeteorPlayfield: public Playfield {

		int load();

		int load(FrameworkConfigManager* f, MeteorConfigManager* m);

	public:

		MeteorPlayfield();

		virtual int Elapse(MTO_FLOAT elapsedTime);

	protected:

		Pitch startPitch;

		int pitchCount;

	};

}
}












#endif