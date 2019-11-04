#ifndef FAKE_DATABASE_CONTEXT_FACTORY_H
#define FAKE_DATABASE_CONTEXT_FACTORY_H


#include "DatabaseContextFactory.h"


namespace Framework {
namespace Database{

	class FakeDatabaseContextFactory : public DatabaseContextFactory {

	public:

		FakeDatabaseContextFactory(GameHost* gHost);

	protected:

		virtual int createDatabaseContext();

	};

}}




#endif