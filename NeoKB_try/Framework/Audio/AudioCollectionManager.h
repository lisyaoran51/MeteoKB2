#ifndef AUDIO_COLLECTION_MANAGER_H
#define AUDIO_COLLECTION_MANAGER_H



#include <vector>
#include <thread>
#include <functional>
#include "AdjustableAudioComponent.h"
#include "../../Util/TemplateConstraint.h"

using namespace std;
using namespace Util;


namespace Framework {
namespace Audio {

	/// <summary>
	/// �̭��i�H�[�J�귽T
	/// TODO: �O��adjust aduio component�A�����n�Oupdatable�~��update
	/// </summary>
	template<class T>
	class AudioCollectionManager : private TConstraint<T, AdjustableAudioComponent>, public AdjustableAudioComponent {

	public:

		AudioCollectionManager();

		int AddItem(T* item);

		int AddItemToList(T* item);

		int RegisterItem(T* item);

		int UnregisterItem(T* item);

		/// <summary>
		/// �o�ӷ|��L�U������Lmanager��s�A�M��audio manager�|��o��update���thread�̭��]
		/// </summary>
		int Update();

	protected:

		vector<T*>* items;

	private:

	};

}}


#endif