#ifndef METEOR_PLAYFIELD_H
#define METEOR_PLAYFIELD_H

#include "../../Games/Scene/Play/Playfield.h"
#include "../../Util/MtoType.h"
#include "../Config/MeteorConfigManager.h"
#include "../../Instruments/Pitch.h"

using namespace Util;
using namespace Games::Scenes::Play;
using namespace Games::Schedulers::Events;
using namespace Meteor::Config;
using namespace Instruments;



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