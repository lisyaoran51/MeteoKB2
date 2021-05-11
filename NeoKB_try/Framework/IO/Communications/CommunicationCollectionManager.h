#ifndef COMMUNICATION_COLLECTION_MANAGER_H
#define COMMUNICATION_COLLECTION_MANAGER_H


#include <thread>
#include "CommunicationComponent.h"


using namespace std;


namespace Framework {
namespace IO{
namespace Communications{

	template<typename T>
	class CommunicationCollectionManager : public TCommunicationComponent<CommunicationRequest> {

		mutable mutex itemMutex;

	public:

		CommunicationCollectionManager(Host* gHost) : TCommunicationComponent(gHost), RegisterType("CommunicationCollectionManager"){
			sourceClock = new StopwatchClock();
		}

		/// <summary>
		/// �o�ӷ|��L�U������Lmanager��s�A�M��audio manager�|��o��update���thread�̭��]
		/// </summary>
		virtual int Update() {
			CommunicationComponent::Update();
			for (int i = 0; i < items.size(); i++) {
				items[i]->Update();
			}
		}

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
		/// �i�o�gaction�A�ҥH�٨S����
		/// </summary>
		virtual int RegisterItem(T* item) {

			dynamic_cast<CommunicationComponent*>(item)->SetSourceClock(sourceClock);
			dynamic_cast<CommunicationComponent*>(item)->InitializeFramedClockAndScheduler();

			return 0;
		}

		virtual int UnregisterItem(T* item) {
			// �i�o�g
			// ��ꤣ�Φ�AAddAdjustmentDependency�S���v�T��~�ɪ�����A�ҥH�����R���N�n
			return 0;
		}

		int SetIsActive(bool iActive) {
			isActive = iActive;
		}

		/*
		int _DebugPrintComponents(string spaces) {

			AudioComponent::_DebugPrintComponents(spaces);
			unique_lock<mutex> uLock(itemMutex);
			for (int i = 0; i < items.size(); i++) {
				items[i]->_DebugPrintComponents(spaces + "--"s);
			}
			return 0;
		}
		*/

		StopwatchClock* GetSourceClock();

		FrameBasedClock* GetFramedClock();


	protected:


		StopwatchClock* sourceClock = nullptr;

		FrameBasedClock* clock = nullptr;

		bool isActive = true;

		vector<T*> items;

		virtual int deleteItem(T* item) {
			UnregisterItem(item);

			DeleteItemFromList(item);

			return 0;
		}



	private:

	};


}}}




#endif