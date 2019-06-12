#ifndef AUDIO_COLLECTION_MANAGER_H
#define AUDIO_COLLECTION_MANAGER_H



#include <vector>
#include <thread>
#include <functional>

using namespace std;


namespace Framework {
namespace Audio {

	/// <summary>
	/// �̭��i�H�[�J�귽T
	/// TODO: �O��adjust aduio component�A�����n�Oupdatable�~��update
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