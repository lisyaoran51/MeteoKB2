#ifndef PATTERN_GENERATOR_H
#define PATTERN_GENERATOR_H



#include <string>
#include <fstream>
#include <vector>
#include <functional>
#include "../../../Util/TemplateConstraint.h"
#include "../../Scheduler/Event/Event.h"
#include "Pattern.h"
#include "../../../Util/MtoObject.h"
#include "../../Config/FrameworkConfigManager.h"
#include "../../../Util/Hierachal/ChildAddable.h"




using namespace std;
using namespace Util;
using namespace Util::Hierachal;
using namespace Base::Schedulers::Events;
using namespace Base::Sheetmusics;
using namespace Base::Config;


namespace Base {
namespace Sheetmusics {
namespace Patterns {

	/// <summary>
	/// to generate special pattern from original events
	///	</summary>
	class PatternGenerator: public ChildAddable
	{

		int load();

		int load(FrameworkConfigManager* fConfigManager);

	public:

		PatternGenerator();

		~PatternGenerator();

		virtual Pattern* Generate(vector<Event*>* es, Event * e) = 0;

		int Add(Pattern* p);

		int CleanPatterns();

		/// <summary>
		/// 之後要移到post processor裡面
		/// </summary>
		virtual int CreateOtherEvent(vector<Event*>* es);

	protected:

		int width;
		int height;
		int targetHeight;
		int blackKeyTargetHeight;
		int blackKeyHeight;

		Pitch startPitch;

		vector<Pattern*> patterns;

		Event* event;

		//Sm<Event>* sm;

	};


}}}







#endif