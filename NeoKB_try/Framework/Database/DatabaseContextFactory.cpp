#include "DatabaseContextFactory.h"

#include "../Hosts/Host.h"

using namespace Framework::Database;



DatabaseContextFactory::DatabaseContextFactory(Host * h)
{
	host = h;
}

int DatabaseContextFactory::Initialize() 
{

	LOG(LogLevel::Depricated) << "DatabaseContextFactory::Initialize() : creating db context.";

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
	LOG(LogLevel::Depricated) << "DatabaseContextFactory::createDatabaseContext() : put storage into context.";
	LOG(LogLevel::Depricated) << "DatabaseContextFactory::createDatabaseContext() : host address = " << host;


	dbContext = new DatabaseContext(host->GetStorage());
	dbContext->Initialize();
	return 0;
}
