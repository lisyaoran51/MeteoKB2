#ifndef EVENT_PARSER_H
#define EVENT_PARSER_H

#include <string>
#include "../../../Scheduler/Event/effect/effect.h"


using namespace std;
using namespace Util;
using namespace Games::Sheetmusic;
using namespace Games::Schedulers::Events::Effects;




namespace Games {
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