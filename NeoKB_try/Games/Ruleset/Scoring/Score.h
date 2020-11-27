#ifndef SCORE_H
#define SCORE_H


#include "../RulesetInfo.h"
#include <vector>
#include "../Modifiers/Modifier.h"
#include "../../Sheetmusic/SheetmusicInfo.h"
#include "../Replays/Replay.h"
#include "../../Users/User.h"
#include "../Judgements/HitResult.h"
#include <map>
#include <ctime>


using namespace Games::Rulesets;
using namespace Games::Rulesets::Modifiers;
using namespace std;
using namespace Games::Sheetmusics;
using namespace Games::Rulesets::Replays;
using namespace Games::Users;
using namespace Games::Rulesets::Judgements;



namespace Games {
namespace Rulesets {
namespace Scoring {

	class Score {

	public:

		Score();

		int hits = 0;

		int maxHits = 0;

		double score = 0;

		double maxScore = 0;

		double accuracy = 0;

		int combo = 0;

		map<HitResult, int> hitResults;

		RulesetInfo* rulesetInfo = nullptr;

		vector<Modifier> modifiers;

		SmInfo* smInfo = nullptr;

		User* user = nullptr;

		Replay* replay = nullptr;

		tm* date = nullptr;

	};




}}}





#endif