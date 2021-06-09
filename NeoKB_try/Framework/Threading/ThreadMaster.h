#ifndef THREAD_MASTER_H
#define THREAD_MASTER_H


#include "../../Util/Singleton.h"
#include <map>
#include <sys/types.h>
#include <thread>
#include <vector>
#include "../Allocation/Hierachal/MtoObject.h"
#include "../../Util/DataStructure/ActionList.h"


using namespace Util;
using namespace Util::DataStructure;
using namespace std;
using namespace Framework::Allocation::Hierachal;


namespace Framework {
namespace Threading {

	class GameThread;
	class SimpleThread;

	class ThreadMaster : public Singleton<ThreadMaster> {

	public:

		ThreadMaster() = default;

		int AddNewThread(GameThread* thread);

		/// <summary>
		/// simple thread�u������L���j�鶡�j�ɶ��A�S����L�\��
		/// </summary>
		int AddSimpleThread(SimpleThread* t);

		GameThread* GetThread(string threadName);

		/// <summary>
		/// simple thread�u������L���j�鶡�j�ɶ��A�S����L�\��
		/// </summary>
		SimpleThread* GetSimpleThread(string threadName);

		int SetThreadProcessStatus(GameThread* thread, bool isProcessing);

		bool CheckThreadProcessable();

		bool CheckThreadProcessable(string threadName);

		int HoldAllThreads();

		int ReleaseAllThreads();

		int Start();

		int SetFrequency(double hz);

		int AddObjectToDelete(MtoObject* oToDelete);

		int AddGameStatusThreadAction(int status, MtoObject* callableObject, function<int()> callback, string callbackName = "ThreadMaster::HandleSwitchGameStatus");

		int SwitchGameStatus(int status);

	protected:

		bool exitRequest = false;

		pid_t threadId = -1;

		double runHz = 100;

		thread* runThread = nullptr;

		/// <summary>
		/// �L���A
		/// �t�����A
		/// �C�����A
		/// �U�����A
		/// �������A
		/// </summary>
		int nowGameStatus = 0;

		map<int, ActionList<int>*> gameStatusThreadActions;

		map<GameThread*, bool> threadProcessing;

		/// <summary>
		/// simple thread�u������L���j�鶡�j�ɶ��A�S����L�\��
		/// </summary>
		map<string, SimpleThread*> simpleThreads;

		bool isHolding = false;

		vector<MtoObject*> objectToDelete;

		int runWork();

	};

}}





#endif