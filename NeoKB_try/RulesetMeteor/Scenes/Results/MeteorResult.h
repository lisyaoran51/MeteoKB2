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

		int load(OutputManager* o, Instrument* i, Storage* s);

		/// <summary>
		/// 將遊戲紀錄寫入檔案步驟一：把遊戲紀錄轉乘control point
		/// </summary>
		int convertToControlPoints(vector<ControlPoint*>& controlPoints);
		
		/// <summary>
		/// 將遊戲紀錄寫入檔案步驟二：標註小節記號
		/// </summary>
		int tagSectionData(vector<ControlPoint*>& controlPoints);

		/// <summary>
		/// 將遊戲紀錄寫入檔案步驟三：轉乘string，寫入sm檔
		/// </summary>
		string encodeToRecordFile(vector<ControlPoint*>& controlPoints);

	public:

		MeteorResult(Score* s);

	protected:

		OutputManager* outputManager = nullptr;

		Piano* piano = nullptr;

		Storage* storage = nullptr;

		string writeGameRecord();

		virtual int onEntering(Scene* lastScene);

	};


}}}





#endif