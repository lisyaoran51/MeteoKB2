#ifndef INSTANT_RESULT_H
#define INSTANT_RESULT_H


#include "../../../Games/Scene/Results/Result.h"
#include "../../../Framework/Output/OutputManager.h"


using namespace Games::Scenes::Results;
using namespace Framework::Output;



namespace Instant {
namespace Scenes{
namespace Results{


	class InstantResult : public Result {

		int load();

		int load(OutputManager* o);

	public:

		InstantResult(Score* s);

	protected:

		OutputManager* outputManager = nullptr;

		virtual int onEntering(Scene* lastScene);

	};


}}}





#endif