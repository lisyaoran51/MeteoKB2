#include "FakeDatabaseContextFactory.h"

#include "FakeMeteoDatabaseContext.h"

using namespace Framework::Database;


FakeDatabaseContextFactory::FakeDatabaseContextFactory(GameHost * gHost): DatabaseContextFactory(gHost)
{
}

int FakeDatabaseContextFactory::createDatabaseContext()
{
	dbContext = new FakeMeteoDatabaseContext(host->GetStorage());
	return 0;
}
