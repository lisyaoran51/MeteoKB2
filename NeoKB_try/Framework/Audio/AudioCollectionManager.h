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
	class AudioCollectionManager : private TConstraint<T, AdjustableAudioComponent>, public AdjustableAudioComponent {

	public:

		int AddItem(T* item);

		int AddItemToList(T* item);

		/// <summary>
		/// 懶得寫action，所以還沒有用
		/// </summary>
		int RegisterItem(T* item);

		int UnregisterItem(T* item);

		

	protected:

		vector<T*> items;

		virtual int deleteItem(T* item);

		/// <summary>
		/// 這個會把他下面的其他manager更新，然後audio manager會把這個update放到thread裡面跑
		/// </summary>
		virtual int update();

	private:

	};


}}


#endif