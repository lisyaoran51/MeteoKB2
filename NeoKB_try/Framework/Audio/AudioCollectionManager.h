#ifndef AUDIO_COLLECTION_MANAGER_H
#define AUDIO_COLLECTION_MANAGER_H



#include <vector>
#include <thread>
#include <functional>
#include "AdjustableAudioComponent.h"
#include "../../Util/TemplateConstraint.h"

using namespace std;
using namespace Util;
using namespace std::literals::string_literals;


namespace Framework {
namespace Audio {

	/// <summary>
	/// �̭��i�H�[�J�귽T
	/// TODO: �O��adjust aduio component�A�����n�Oupdatable�~��update
	/// </summary>
	template<class T>
	class AudioCollectionManager : public AdjustableAudioComponent {

		mutable mutex itemMutex;

	public:

		int AddItem(T* item) {
			RegisterItem(item);
			AddItemToList(item);
			return 0;
		}

		int AddItemToList(T* item) {

			unique_lock<mutex> uLock(itemMutex);
			items.push_back(item);
			return 0;
		}

		/// <summary>
		/// �i�o�gaction�A�ҥH�٨S����
		/// </summary>
		int RegisterItem(T* item) {

			pendingActions.Add(this, [=]() {

				item->AddAdjustmentDependency(this);

			}, "AudioCollectionManager::RegisterItemAdjustments");

			return 0;
		}

		int UnregisterItem(T* item) {
			// �i�o�g
			return 0;
		}

		/// <summary>
		/// �o�ӷ|��L�U������Lmanager��s�A�M��audio manager�|��o��update���thread�̭��]
		/// </summary>
		virtual int Update() {

			AudioComponent::Update();

			unique_lock<mutex> uLock(itemMutex);
			int itemsSize = items.size();
			uLock.unlock();

			LOG(LogLevel::Finest) << "AudioCollectionManager::Update() : this = [" << this << "], item size = [" << itemsSize << "].";

			for (int i = 0; i < itemsSize; i++) {

				uLock.lock();
				T* item = dynamic_cast<T*>(items[i]);
				uLock.unlock();

				if (item == nullptr)
					throw runtime_error("AudioCollectionManager<T>::update: this component's type is wrong.");

				if (item->GetIsCompleted()) {

					uLock.lock();
					items.erase(items.begin() + i);
					uLock.unlock();

					i--;
					deleteItem(item);
					continue;
					// smaple����delete�A���Otrack�ndelete
				}
				item->Update();
			}

			return 0;
		}

		int _DebugPrintComponents(string spaces) {

			AudioComponent::_DebugPrintComponents(spaces);
			unique_lock<mutex> uLock(itemMutex);
			for (int i = 0; i < items.size(); i++) {
				items[i]->_DebugPrintComponents(spaces + "--"s);
			}
			return 0;
		}

	protected:

		vector<T*> items;

		virtual int deleteItem(T* item) {
			return 0;
		}

		

	private:

	};


}}


#endif