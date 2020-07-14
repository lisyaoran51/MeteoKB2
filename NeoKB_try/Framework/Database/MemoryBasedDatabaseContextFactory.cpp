#include "MemoryBasedDatabaseContextFactory.h"


using namespace Framework::Database;


MemoryBasedDatabaseContextFactory::MemoryBasedDatabaseContextFactory(): DatabaseContextFactory(nullptr)
{
}

int MemoryBasedDatabaseContextFactory::createDatabaseContext()
{
	return 0;
}
