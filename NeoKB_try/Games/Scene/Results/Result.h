#ifndef RESULT_H
#define RESULT_H


#include "../../Ruleset/Scoring/Score.h"
#include "../MeteoScene.h"


using namespace Games::Rulesets::Scoring;
using namespace Games::Scenes;


namespace Games {
namespace Scenes{
namespace Results{

	class Result : public MeteoScene {

	public:

		Result(Score* s);


	protected:

		Score* score = nullptr;

		/* -------------------Scene------------------- */

		virtual int onEntering(Scene* lastScene);

		virtual int onExiting(Scene* lastScene);

	};

}}}




#endif