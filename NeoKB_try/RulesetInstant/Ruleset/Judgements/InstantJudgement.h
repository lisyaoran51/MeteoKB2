#ifndef METEOR_JUDGEMENT_H
#define METEOR_JUDGEMENT_H


#include "../../../Games/Ruleset/Judgements/Judgement.h"


using namespace Games::Rulesets::Judgements;

namespace Meteor {
namespace Rulesets{
namespace Judgements{

	class MeteorJudgement : public Judgement {

	public:

		MeteorJudgement(HitResult r);

	protected:

		virtual double getResultNumber(HitResult hResult);

	};

}}}





#endif