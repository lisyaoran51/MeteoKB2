#include "SheetmusicStore.h"

#include <stdexcept>

using namespace Games::Sheetmusics;
using namespace std;



SheetmusicStore::SheetmusicStore(function<DatabaseContext*(void)> getContext): DatabaseStore(getContext), RegisterType("SheetmusicStore")
{
}

int SheetmusicStore::Add(SmInfo * smInfo)
{
	DatabaseContext* context = getContext();

	// TODO: database�g������A�[�o��
	//context->AddDbSetEntity<SmInfo>(smInfo);

	return 0;
}

int SheetmusicStore::Delete(SmInfo * smInfo)
{
	throw runtime_error("SheetmusicStore::Delete(): not implemented.");
	return 0;
}

vector<SmInfo*>* SheetmusicStore::GetSheetmusics()
{
	DatabaseContext* context = getContext();
	return context->GetSmInfos()->GetEntities();
}
