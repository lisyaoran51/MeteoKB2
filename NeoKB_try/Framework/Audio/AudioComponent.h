#ifndef AUDIO_COMPONENT_H
#define AUDIO_COMPONENT_H



#include <vector>
#include <thread>
#include <functional>

using namespace std;


namespace Framework {
namespace Audio {

	/// <summary>
	/// �̭���work queue�A�C��update�N��u�@����
	/// </summary>
	template<T>
	class AudioComponent : Updatable {

	public:

		AudioComponent();

		int Update();

	protected:

		/// <summary>
		/// Audio operations will be run on a separate dedicated thread, so we need to schedule any audio API calls using this queue.
		/// </summary>
		// protected ConcurrentQueue<Action> PendingActions = new ConcurrentQueue<Action>();

	private:

	};

}}


#endif