#include "../Framework/Timing/StopwatchClock.h"
#include "../Util/Log.h"
#include <iomanip>


using namespace Framework::Timing;
using namespace Util;
using namespace std;


LogLevel Util::logLevel = LogLevel::Finest;


int main() {

	StopwatchClock s;

	while(1)
		LOG(LogLevel::Info) << "TestCaseStopwatch() : pass " << fixed << setprecision(5) << s.GetCurrentTime() << " s";

	return 0;
}