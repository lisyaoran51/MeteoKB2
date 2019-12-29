#ifndef OUTPUT_COLLECTION_MANAGER_H
#define OUTPUT_COLLECTION_MANAGER_H


#include "OutputComponent.h"
#include <vector>


using namespace std;


namespace Framework {
namespace Output{

	template<typename T>
	class OutputCollectionManager: public OutputComponent {

	public:

		virtual int AddItem(T* item) {
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
		/// 懶得寫action，所以還沒有用
		/// </summary>
		int RegisterItem(T* item) {
			// 懶得寫
			return 0;
		}

		virtual int PushMessage(OutputMessage* outputMessage) {

			unique_lock<mutex> uLock(itemMutex);

			for (int i = 0; i < items.size(); i++) {
				items[i]->PushMessage(outputMessage);
			}

		}

	protected:

		mutable mutex itemMutex;


		vector<T*> items;

		virtual int deleteItem(T* item) {
			return 0;
		}

	};

}}






#endif
