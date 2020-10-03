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
	/// 裡面可以加入資源T
	/// TODO: 記成adjust aduio component，必須要是updatable才能update
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

		int DeleteItemFromList(T* item) {

			unique_lock<mutex> uLock(itemMutex);
			for (int i = 0; i < items.size(); i++) {
				if (items[i] == item) {
					items.erase(items.begin() + i);
					return 0;
				}

			}
			return -1;
		}

		/// <summary>
		/// 懶得寫action，所以還沒有用
		/// </summary>
		int RegisterItem(T* item) {
			item->SetClock(clock);

			pendingActions.Add(this, [=]() {

				item->AddAdjustmentDependency(this);

				return 0;
			}, "AudioCollectionManager::RegisterItemAdjustments");

			return 0;
		}

		int UnregisterItem(T* item) {
			// 懶得寫
			// 其實不用血，AddAdjustmentDependency沒有影響到外界的物件，所以直接刪掉就好
			return 0;
		}


		virtual int OnStateChange() {
			AdjustableAudioComponent::OnStateChange();
			unique_lock<mutex> uLock(itemMutex);
			for (int i = 0; i < items.size(); i++) {
				items[i]->OnStateChange();
			}
		}

		/// <summary>
		/// 這個會把他下面的其他manager更新，然後audio manager會把這個update放到thread裡面跑
		/// </summary>
		virtual int Update() {

			if (!isActive)
				return 0;

			AudioComponent::Update();


			LOG(LogLevel::Finest) << "AudioCollectionManager::Update() : this = [" << this << "], item size = [" << items.size() << "].";

			for (int i = 0; i < items.size(); i++) {

				T* item = dynamic_cast<T*>(items[i]);

				if (item == nullptr)
					throw runtime_error("AudioCollectionManager<T>::update: this component's type is wrong.");

				/* 這個應該要用pending action來做。現在沒有在用track，應該不用寫這段
				if (item->GetIsCompleted()) {

					items.erase(items.begin() + i);

					i--;
					deleteItem(item);
					continue;
					// smaple不用delete，但是track要delete
				}
				*/

				item->Update();
			}

			return 0;
		}

		int SetIsActive(bool iActive) {
			isActive = iActive;
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

		bool isActive = true;

		vector<T*> items;

		virtual int deleteItem(T* item) {
			UnregisterItem();

			unique_lock<mutex> uLock(itemMutex);

			DeleteItemFromList(item);
			
			return 0;
		}

		

	private:

	};


}}


#endif