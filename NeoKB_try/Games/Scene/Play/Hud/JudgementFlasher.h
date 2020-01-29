#ifndef JUDGEMENT_FLASHER_H
#define JUDGEMENT_FLASHER_H


#include "../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../Ruleset/Judgements/Judgement.h"
#include "../../../../Framework/Output/OutputManager.h"


using namespace Framework::Allocation::Hierachal;
using namespace Games::Rulesets::Judgements;
using namespace Framework::Output;


namespace Games {
namespace Scenes {
namespace Play {
namespace Hud {


	class JudgementFlasher : public Container {

		OutputManager* outputManager;

		int load();

	public:

		JudgementFlasher();

		int FlashJudgement(Judgement* judgement);

	};


}}}}





#endif