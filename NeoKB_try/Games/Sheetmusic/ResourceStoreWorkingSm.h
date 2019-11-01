#ifndef RESOURCE_STORE_WORKING_SM_H
#define RESOURCE_STORE_WORKING_SM_H


#include "WorkingSheetmusic.h"
#include "../../Framework/IO/Stores/ResourceStore.h"



using namespace Framework::IO::Stores;



namespace Games {
namespace Sheetmusics {

	class ResourceStoreWorkingSm : public WorkingSm {

		ResourceStore<char*>* resourceStore;

	public:

		ResourceStoreWorkingSm(ResourceStore<char*>* rStore, SmInfo* sInfo);

		virtual Sm<Event>* GetSm();

	protected:

		virtual string getPathForFile(string fileName);


	};



}}



#endif