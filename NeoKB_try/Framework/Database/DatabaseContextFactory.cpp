#include "DatabaseContextFactory.h"

#include "../Host/GameHost.h"

using namespace Framework::Database;



DatabaseContextFactory::DatabaseContextFactory(GameHost * h)
{
	host = h;
}

int DatabaseContextFactory::Initialize() {

	createDatabaseContext();
	initialized = true;
	return 0;
}

DatabaseContext * DatabaseContextFactory::GetContext()
{
	if (!initialized)
		throw runtime_error("DatabaseContextFactory::GetContext(): not initialized.");
	return dbContext;
}

int DatabaseContextFactory::createDatabaseContext()
{
	dbContext = new DatabaseContext(host->GetStorage());
	dbContext->Initialize();
	return 0;
}
