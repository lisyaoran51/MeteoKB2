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
	class AudioCollectionManager : public AdjustableAudioComponent {

	public:

		int AddItem(T* item) {
			RegisterItem(item);
			AddItemToList(item);
			return 0;
		}

		int AddItemToList(T* item) {
			items.push_back(item);
			return 0;
		}

		/// <summary>
		/// �i�o�gaction�A�ҥH�٨S����
		/// </summary>
		int RegisterItem(T* item) {
			// �i�o�g
			return 0;
		}

		int UnregisterItem(T* item) {
			// �i�o�g
			return 0;
		}

		

	protected:

		vector<T*> items;

		virtual int deleteItem(T* item) {
			return 0;
		}

		/// <summary>
		/// �o�ӷ|��L�U������Lmanager��s�A�M��audio manager�|��o��update���thread�̭��]
		/// </summary>
		virtual int update() {
			AdjustableAudioComponent::update();

			for (int i = 0; i < items.size(); i++) {
				T* item = dynamic_cast<T*>(items[i]);

				if (item == nullptr)
					throw runtime_error("AudioCollectionManager<T>::update: this component's type is wrong.");

				if (item->GetIsCompleted()) {
					items.erase(items.begin() + i);
					i--;
					deleteItem(item);
					// smaple����delete�A���Otrack�ndelete
				}
			}

			return 0;
		}

	private:

	};


}}


#endif