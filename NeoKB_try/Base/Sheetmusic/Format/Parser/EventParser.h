#ifndef EVENT_PARSER_H
#define EVENT_PARSER_H

#include <string>
#include "../../../Scheduler/Event/effect/effect.h"


using namespace std;
using namespace Util;
using namespace Base::Sheetmusic;
using namespace Base::Schedulers::Events::Effects;




namespace Base {
namespace Sheetmusic {
namespace format {
namespace Parsers {


	class EventParser
	{

	public:

		virtual Event* Parse(string text) = 0;

	};



}}}}



#endif