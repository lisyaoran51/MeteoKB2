#ifndef THREAD_MASTER_H
#define THREAD_MASTER_H


#include "../../Util/Singleton.h"
#include <map>
#include <sys/types.h>
#include <thread>
#include <vector>
#include "../Allocation/Hierachal/MtoObject.h"


using namespace Util;
using namespace std;
using namespace Framework::Allocation::Hierachal;


namespace Framework {
namespace Threading {

	class ThreadMaster : public Singleton<ThreadMaster> {

	public:

		ThreadMaster() = default;

		int AddNewThread(string threadName);

		int SetThreadProcessStatus(string threadName, bool isProcessing);

		bool CheckThreadProcessable();

		bool CheckThreadProcessable(string threadName);

		int HoldAllThreads();

		int ReleaseAllThreads();

		int Start();

		int SetFrequency(double hz);

		int AddObjectToDelete(MtoObject* oToDelete);

	protected:

		bool exitRequest = false;

		pid_t threadId = -1;

		double runHz = 100;

		thread* runThread = nullptr;

		map<string, bool> threadProcessing;

		bool isHolding = false;

		vector<MtoObject*> objectToDelete;

		int runWork();

	};

}}





#endif