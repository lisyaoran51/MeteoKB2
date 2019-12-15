#ifndef METEOR_HIT_WINDOW_H
#define METEOR_HIT_WINDOW_H

#include "../../../Games/Ruleset/Judgements/HitWindow.h"


using namespace Games::Rulesets::Judgements;


namespace Meteor {
namespace Rulesets{
namespace Judgements{

	class MeteorHitWindow: public HitWindow{

	public:

		virtual HitResult GetResultFor(double hitOffset);

		virtual double GetHitWindow(HitResult hResult);

	};

}
}
}




#endif