#ifndef METEOR_RESULT_H
#define METEOR_RESULT_H


#include "../../../Games/Scene/Results/Result.h"
#include "../../../Instruments/Piano.h"
#include "../../../Games/Scheduler/Event/ControlPoints/ControlPoint.h"
#include "../../../Framework/IO/Communications/CommunicationAccess.h"


using namespace Games::Scenes::Results;
using namespace Instruments;
using namespace Games::Schedulers::Events::ControlPoints;
using namespace Framework::IO::Communications;



namespace Meteor {
namespace Scenes{
namespace Results{


	class MeteorResult : public Result {

		int load();

		int load(Instrument* i, Storage* s, CommunicationAccess* c);

		/// <summary>
		/// 去0，
		/// </summary>
		string TrimOff(string s, int precision);

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

		Piano* piano = nullptr;

		Storage* storage = nullptr;

		CommunicationAccess* communicationAccess = nullptr;

		string writeGameRecord();

		virtual int onEntered(Scene* lastScene);

		virtual int onMessage(MeteoBluetoothMessage* message);

	};


}}}





#endif