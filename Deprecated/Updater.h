#ifndef UPDATER_H
#define UPDATER_H


#include <vector>
#include <chrono>
#include"../MtoType.h"
#include "../MtoObject.h"

using namespace std::chrono;
using namespace std;
using namespace Util;



/*
 * 取得相差時間
 * http://zh.cppreference.com/w/cpp/chrono/system_clock/now
 */



namespace Util {
namespace Update {

	enum class UpdateState;

	/// <summary>
	/// to update current time for every object who need to update.
	/// <summary>
	class Updater: public MtoObject{

		system_clock::time_point currentTime;

		system_clock::time_point startTime;

		vector<function<int(MTO_FLOAT)>> tasks;

		void update(MTO_FLOAT elapsedTime);

		UpdateState updateState;

	public:

		Updater();

		virtual ~Updater() = default;

		int RegisterTask(function<int(MTO_FLOAT)> t);

		// int RemoveTask(MTO_FUNC_POINTER t);

		void Update();

		int SetUpdateState(UpdateState uState);
	};


	enum class UpdateState {

		NotStarted,
		Started,
		Paused,
		Stopped

	};


}}

#endif