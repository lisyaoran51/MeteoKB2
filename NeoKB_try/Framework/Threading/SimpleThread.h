#ifndef SIMPLE_THREAD_H
#define SIMPLE_THREAD_H


#include <thread>
#include <sys/types.h>
#include "../Allocation/Hierachal/MtoObject.h"


using namespace std;
using namespace Framework::Allocation::Hierachal;



/* 
 * ®³pidªº¤èªk
 * http://man7.org/linux/man-pages/man2/syscall.2.html
 * https://stackoverflow.com/questions/15708983/how-can-you-get-the-linux-thread-id-of-a-stdthread
 */


namespace Framework {
namespace Threading {

	class SimpleThread : virtual public RegisterType {

	public:

		SimpleThread();

		int SetSleepTimeInMilliSecond(int sTimeInMilliSecond);

	protected:

		thread* thisThread = nullptr;

		int sleepTimeInMilliSecond = 1;

	};

}}


#endif