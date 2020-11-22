#ifndef METEOR_RESULT_H
#define METEOR_RESULT_H


#include "../../../Games/Scene/Results/Result.h"
#include "../../../Framework/Output/OutputManager.h"
#include "../../../Instruments/Piano.h"
#include "../../../Games/Scheduler/Event/ControlPoints/ControlPoint.h"


using namespace Games::Scenes::Results;
using namespace Framework::Output;
using namespace Instruments;
using namespace Games::Schedulers::Events::ControlPoints;



namespace Meteor {
namespace Scenes{
namespace Results{


	class MeteorResult : public Result {

		int load();

		int load(OutputManager* o, Instrument* i);

		int convertToControlPoints(vector<ControlPoint*>& controlPoints);
		
		int tagSectionData(vector<ControlPoint*>& controlPoints);

		string encodeToRecordFile(vector<ControlPoint*>& controlPoints);

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