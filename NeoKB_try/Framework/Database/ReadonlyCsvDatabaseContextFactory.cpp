#include "ReadonlyCsvDatabaseContextFactory.h"

#include "FakeMeteoDatabaseContext.h"
#include "../Hosts/Host.h"

using namespace Framework::Database;
using namespace Framework::Hosts;

ReadonlyCsvDatabaseContextFactory::ReadonlyCsvDatabaseContextFactory(Host * gHost): DatabaseContextFactory(gHost)
{
}

int ReadonlyCsvDatabaseContextFactory::createDatabaseContext()
{
	dbContext = new FakeMeteoDatabaseContext(host->GetStorage());
	dbContext->Initialize();
	return 0;
}
