#ifndef UPDATABLE_H
#define UPDATABLE_H


#include <vector>
#include "../../Util/MtoType.h"

using namespace std;


namespace Util {
namespace Update {

	/// <summary>
	/// every class inherit form this has a function to update. we register update() to the updater. updater will call update().
	/// </summary>
	class Updatable {

	public:

		Updatable();

		virtual int Elapse(MTO_FLOAT elapsedTime) = 0;

		MTO_FLOAT GetCurrentTime();

	protected:

		MTO_FLOAT currentTime;

	};

}}

#endif