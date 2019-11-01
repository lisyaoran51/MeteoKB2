#include "DatabaseContextFactory.h"

#include "../Host/GameHost.h"

using namespace Framework::Database;



DatabaseContextFactory::DatabaseContextFactory(GameHost * h)
{
	host = h;
	dbContext = new DatabaseContext(host->GetStorage());
}

DatabaseContext * DatabaseContextFactory::GetContext()
{
	return dbContext;
}
