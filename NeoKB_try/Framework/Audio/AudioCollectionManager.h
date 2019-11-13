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
	/// 裡面可以加入資源T
	/// TODO: 記成adjust aduio component，必須要是updatable才能update
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
		/// 懶得寫action，所以還沒有用
		/// </summary>
		int RegisterItem(T* item) {
			// 懶得寫
			return 0;
		}

		int UnregisterItem(T* item) {
			// 懶得寫
			return 0;
		}

		

	protected:

		vector<T*> items;

		virtual int deleteItem(T* item) {
			return 0;
		}

		/// <summary>
		/// 這個會把他下面的其他manager更新，然後audio manager會把這個update放到thread裡面跑
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
					// smaple不用delete，但是track要delete
				}
			}

			return 0;
		}

	private:

	};


}}


#endif