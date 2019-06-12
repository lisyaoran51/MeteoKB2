#ifndef AUDIO_COLLECTION_MANAGER_H
#define AUDIO_COLLECTION_MANAGER_H



#include <vector>
#include <thread>
#include <functional>

using namespace std;


namespace Framework {
namespace Audio {

	/// <summary>
	/// 裡面可以加入資源T
	/// TODO: 記成adjust aduio component，必須要是updatable才能update
	/// </summary>
	template<T>
	class AudioCollectionManager : private TConstraint<T, AdjustableAudioComponent>, AdjustableAudioComponent {

	public:

		AudioCollectionManager();

		int AddItem(T* item);

		int AddItemToList(T* item);

		int RegisterItem(T* item);

		int UnregisterItem(T* item);

		int Update();

	protected:

		vector<T*>* items;

	private:

	};

}}


#endif