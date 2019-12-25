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
		/// �i�o�gaction�A�ҥH�٨S����
		/// </summary>
		int RegisterItem(T* item) {
			// �i�o�g
			return 0;
		}

		virtual int PushMessage(OutputMessage* outputMessage) {

			for (int i = 0; i < items.size(); i++) {
				items[i]->PushMessage(outputMessage);
			}

		}

	protected:

		vector<T*> items;

		virtual int deleteItem(T* item) {
			return 0;
		}

	};

}}






#endif
