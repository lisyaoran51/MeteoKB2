#ifndef READONLY_CSV_DATABASE_CONTEXT_FACTORY_H
#define READONLY_CSV_DATABASE_CONTEXT_FACTORY_H


#include "DatabaseContextFactory.h"


namespace Framework {
namespace Database{

	class ReadonlyCsvDatabaseContextFactory : public DatabaseContextFactory {

	public:

		ReadonlyCsvDatabaseContextFactory(GameHost* gHost);

	protected:

		virtual int createDatabaseContext();

	};

}}




#endif