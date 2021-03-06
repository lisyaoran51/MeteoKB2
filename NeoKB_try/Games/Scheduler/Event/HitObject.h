#ifndef HIT_OBJECT_H
#define HIT_OBJECT_H


#include "EventProcessor.h"
#include "../../Ruleset/Judgements/Judgement.h"
#include "../../Ruleset/Judgements/HitWindow.h"
#include "ControlPoints/PlayableControlPoint.h"
#include "ArmedState.h"
#include <vector>




using namespace Games::Rulesets::Judgements;
using namespace std;
using namespace Games::Schedulers::Events::ControlPoints;



namespace Games {
namespace Schedulers{
namespace Events{

	class HitObject : virtual public EventProcessor<Event> {

		bool hasJudgementResult = false;

		bool judgementOccurred = false;

		ActionList<int, HitObject*, Judgement*> onJudgement;

		ArmedState state = ArmedState::Idle;

		vector<Judgement*> judgements;

		int setState(ArmedState s);

	public:

		HitObject(HitWindow* hWindow);
		~HitObject();

		/// <summary>
		/// register the Event to be processed.
		/// </summary>
		virtual EventProcessor<Event>* RegisterEvent(Event* e);

		template<class _Type>
		int AddOnJudgement(_Type* callableObject, function<int(HitObject*, Judgement*)> callback, string name = "HandleJudgement") {
			

			onJudgement.Add(callableObject, callback, name);
			LOG(LogLevel::Finest) << "HitObject::AddOnJudgement : 4";
			LOG(LogLevel::Finest) << "HitObject::AddOnJudgement : add [" << name << "].";
			return 0;
		}

		bool GetHasJudgementResult();

		/// <summary>
		/// 看是不是最後一個範圍內的hit object，只有最後一個hit object需要judgement
		/// 越接近0越好，但是等於0就是超過
		/// </summary>
		virtual double TryJudgement() = 0;

		bool UpdateJudgement(bool isUserTriggered);

		virtual Judgement* GetBestJudgement() = 0;

	protected:

		HitWindow* hitWindow = nullptr;

		virtual HitWindow* createHitWindow(SmDifficulty* smDifficulty) = 0;

		HitWindow* getHitWindow();

		virtual int CheckForJudgement(bool isUserTriggered, double offset) = 0;


		int AddJudgement(Judgement* judgement);

		virtual Judgement* createJudgement(HitResult hResult) = 0;

	};

}}}






#endif
