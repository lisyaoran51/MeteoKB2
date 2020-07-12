#include "ReadonlyCsvDatabaseContextFactory.h"

#include "FakeMeteoDatabaseContext.h"
#include "../Host/GameHost.h"

using namespace Framework::Database;
using namespace Framework::Host;

ReadonlyCsvDatabaseContextFactory::ReadonlyCsvDatabaseContextFactory(GameHost * gHost): DatabaseContextFactory(gHost)
{
}

int ReadonlyCsvDatabaseContextFactory::createDatabaseContext()
{
	dbContext = new FakeMeteoDatabaseContext(host->GetStorage());
	dbContext->Initialize();
	return 0;
}
