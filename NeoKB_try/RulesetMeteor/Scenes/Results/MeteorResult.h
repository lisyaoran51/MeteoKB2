#ifndef METEOR_RESULT_H
#define METEOR_RESULT_H


#include "../../../Games/Scene/Results/Result.h"
#include "../../../Framework/Output/OutputManager.h"
#include "../../../Instruments/Piano.h"


using namespace Games::Scenes::Results;
using namespace Framework::Output;
using namespace Instruments;



namespace Meteor {
namespace Scenes{
namespace Results{


	class MeteorResult : public Result {

		int load();

		int load(OutputManager* o, Instrument* i);

	public:

		MeteorResult(Score* s);

	protected:

		OutputManager* outputManager = nullptr;

		Piano* piano = nullptr;

		string writeGameRecord();

		virtual int onEntering(Scene* lastScene);

	};


}}}





#endif