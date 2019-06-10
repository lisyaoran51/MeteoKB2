#ifndef SHEETMUSIC_CONVERTER_H
#define SHEETMUSIC_CONVERTER_H

#include <string>
#include <fstream>
#include <vector>
#include "WorkingSheetmusic.h"
#include "../../Util/TemplateConstraint.h"
#include "SheetmusicInfo.h"
#include "Sheetmusic.h"
#include "../Scheduler/Event/Event.h"
#include "../../Util/MtoObject.h"
#include "Pattern/PatternGenerator.h"
#include "../../Util/Hierachal/ChildAddable.h"




using namespace std;
using namespace Util;
using namespace Util::Hierachal;
using namespace Base::Schedulers::Events;
using namespace Base::Sheetmusics::Patterns;

namespace Base {
namespace Sheetmusics {

	/// <summary>
	/// when Ruleset executor load in sm, this converter converts the events in sm.
	///	</summary>
	class SmConverter
	{

	public:
		/// <summary>
		/// 讀取pattern generator
		///	</summary>
		SmConverter(PatternGenerator* pg);

		/// <summary>
		/// converts the events in sm
		/// We always operate on a clone of the original sm, to not modify it game-wide
		///	</summary>
		virtual Sm<Event>* Convert(Sm<Event>* s);

	protected:

		/// <summary>
		/// 用來把一個event變成各種特效
		///	</summary>
		PatternGenerator* patternGenerator;

		/// <summary>
		/// 把event付智，並且轉換成各種特效
		///	</summary>
		virtual int convertEvent(vector<Event*>* es, Event* e);

	};


}
}



#endif