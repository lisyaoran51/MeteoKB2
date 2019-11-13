#include "FakeDatabaseContextFactory.h"

#include "FakeMeteoDatabaseContext.h"
#include "../Host/GameHost.h"

using namespace Framework::Database;
using namespace Framework::Host;

FakeDatabaseContextFactory::FakeDatabaseContextFactory(GameHost * gHost): DatabaseContextFactory(gHost)
{
}

int FakeDatabaseContextFactory::createDatabaseContext()
{
	dbContext = new FakeMeteoDatabaseContext(host->GetStorage());
	return 0;
}
