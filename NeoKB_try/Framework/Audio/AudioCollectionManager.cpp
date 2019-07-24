#include "AudioCollectionManager.h"

using namespace Framework::Audio;


template<class T>
int AudioCollectionManager<T>::AddItem(T * item)
{
	RegisterItem(item);
	AddItemToList(item);
	return 0;
}

template<class T>
int AudioCollectionManager<T>::AddItemToList(T * item)
{
	items.push_back(item);
	return 0;
}

template<class T>
int AudioCollectionManager<T>::RegisterItem(T * item)
{
	// 懶得寫
	return 0;
}

template<class T>
int AudioCollectionManager<T>::UnregisterItem(T * item)
{
	// 懶得寫
	return 0;
}

template<class T>
int AudioCollectionManager<T>::deleteItem(T * item)
{
	return 0;
}

template<class T>
int AudioCollectionManager<T>::update()
{
	AdjustableAudioComponent::update();

	for (int i = 0; i < items.size(); i++) {
		AdjustableAudioComponent* item = dynamic_cast<AdjustableAudioComponent*>(items[i]);

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
