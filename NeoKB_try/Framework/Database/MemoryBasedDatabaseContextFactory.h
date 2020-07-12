#ifndef MEMORY_BASED_DATABASE_CONTEXT_FACTORY_H
#define MEMORY_BASED_DATABASE_CONTEXT_FACTORY_H


#include "DatabaseContextFactory.h"


namespace Framework {
namespace Database{

	class MemoryBasedDatabaseContextFactory : public DatabaseContextFactory {

	public:

		MemoryBasedDatabaseContextFactory();

	protected:

		virtual int createDatabaseContext();

	};

}}




#endif