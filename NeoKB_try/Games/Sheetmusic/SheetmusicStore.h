#ifndef SHEETMUSIC_STORE_H
#define SHEETMUSIC_STORE_H


#include "../../Framework/Database/DatabaseStore.h"
#include "SheetmusicInfo.h"


using namespace Framework::Database;



namespace Games {
namespace Sheetmusics{

	class SheetmusicStore : public DatabaseStore {

	public:

		SheetmusicStore(function<DatabaseContext*(void)> getContext);

		int Add(SmInfo* smInfo);

		int Delete(SmInfo* smInfo);

		/// <summary>
		/// �Χ��O�o�n�R���A���M�|memory leak
		/// </summary>
		vector<SmInfo*>* GetSheetmusics();


	};


}}




#endif