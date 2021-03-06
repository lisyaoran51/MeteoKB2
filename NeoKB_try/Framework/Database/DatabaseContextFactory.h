#ifndef DATABASE_CONTEXT_FACTORY_H
#define DATABASE_CONTEXT_FACTORY_H


#include "DatabaseContext.h"


namespace Framework {
namespace Host{
	class GameHost;
}}


using namespace Framework::Host;


namespace Framework {
namespace Database{

	class DatabaseContextFactory {

	public:

		DatabaseContextFactory(GameHost* h);

		int Initialize();

		DatabaseContext* GetContext();

	protected:

		bool initialized = false;

		GameHost* host;

		DatabaseContext* dbContext;

		virtual int createDatabaseContext();

	};

}}





#endif