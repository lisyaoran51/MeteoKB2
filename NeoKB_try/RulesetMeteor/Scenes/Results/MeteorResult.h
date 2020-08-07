#ifndef METEOR_RESULT_H
#define METEOR_RESULT_H


#include "../../../Games/Scene/Results/Result.h"
#include "../../../Framework/Output/OutputManager.h"


using namespace Games::Scenes::Results;
using namespace Framework::Output;



namespace Meteor {
namespace Scenes{
namespace Results{


	class MeteorResult : public Result {

		int load();

		int load(OutputManager* o);

	public:

		MeteorResult(Score* s);

	protected:

		OutputManager* outputManager = nullptr;

		virtual int onEntering(Scene* lastScene);

	};


}}}





#endif