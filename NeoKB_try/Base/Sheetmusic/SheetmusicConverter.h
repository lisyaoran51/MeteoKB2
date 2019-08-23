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
#include "Pattern/PatternGenerator.h"




using namespace std;
using namespace Util;
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
		/// Ū��pattern generator
		///	</summary>
		SmConverter(PatternGenerator* pg);

		/// <summary>
		/// converts the events in sm
		/// We always operate on a clone of the original sm, to not modify it game-wide
		///	</summary>
		virtual Sm<Event>* Convert(Sm<Event>* s);

	protected:

		/// <summary>
		/// �Ψӧ�@��event�ܦ��U�دS��
		///	</summary>
		PatternGenerator* patternGenerator;

		/// <summary>
		/// ��event�I���A�åB�ഫ���U�دS��
		///	</summary>
		virtual int convertEvent(vector<Event*>* es, Event* e);

	};


}
}



#endif