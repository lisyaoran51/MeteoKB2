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
		/// �h0�A
		/// </summary>
		string TrimOff(string s, int precision);

		/// <summary>
		/// �N�C�������g�J�ɮרB�J�@�G��C�������୼control point
		/// </summary>
		int convertToControlPoints(vector<ControlPoint*>& controlPoints);
		
		/// <summary>
		/// �N�C�������g�J�ɮרB�J�G�G�е��p�`�O��
		/// </summary>
		int tagSectionData(vector<ControlPoint*>& controlPoints);

		/// <summary>
		/// �N�C�������g�J�ɮרB�J�T�G�୼string�A�g�Jsm��
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