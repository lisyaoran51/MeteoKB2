#ifndef JUDGEMENT_FLASHER_H
#define JUDGEMENT_FLASHER_H


#include "../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../Ruleset/Judgements/Judgement.h"


using namespace Framework::Allocation::Hierachal;
using namespace Games::Rulesets::Judgements;


namespace Games {
namespace Scenes {
namespace Play {
namespace Hud {


	class JudgementFlasher : public Container {

	public:

		int FlashJudgement(Judgement* judgement);

	};


}}}}





#endif