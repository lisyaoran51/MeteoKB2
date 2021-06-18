#ifndef RECORD_RESULT_H
#define RECORD_RESULT_H


#include "../../../Games/Scene/Results/Result.h"
#include "../../../Framework/Output/OutputManager.h"


using namespace Games::Scenes::Results;
using namespace Framework::Output;



namespace Record {
namespace Scenes{
namespace Results{


	class RecordResult : public Result {

		int load();

	public:

		RecordResult(Score* s);

	protected:

		virtual int onEntering(Scene* lastScene);

	};


}}}





#endif