#ifndef ROLLING_COUNTER_H
#define ROLLING_COUNTER_H


#include "../../../../Framework/Allocation/Hierachal/Container.h"
#include "../../../../Util/DataStructure/ActionList.h"
#include "../../../../Util/DataStructure/Bindable.h"
#include "../../../../Util/Effects/Easing.h"


using namespace Framework::Allocation::Hierachal;
using namespace Util::DataStructure;
using namespace Util::Effects;


namespace Games {
namespace Scenes {
namespace Play {
namespace Hud {

	template<typename T>
	class RollingCounter : public Container {

	public:

		RollingCounter(T cValue): RegisterType("RollingCounter") {
			currentValue = new Bindable<T>(cValue);
		}

		Bindable<T>* GetCurrentValue() {
			return currentValue;
		}

		int Increment(T amount) {
			
			currentValue->SetValue(currentValue->GetValue() + amount);

			return 0;
		}



	protected:

		Bindable<T>* currentValue = nullptr;

		Easing easing = Easing::None;

		/// <summary>
		/// roll¦h¤[
		/// </summary>
		double rollingDuration = 0;

	};

}}}}







#endif