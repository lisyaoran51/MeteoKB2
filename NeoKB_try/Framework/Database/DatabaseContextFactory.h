#ifndef DATABASE_CONTEXT_FACTORY_H
#define DATABASE_CONTEXT_FACTORY_H


#include "DatabaseContext.h"


namespace Framework {
namespace Hosts{
	class Host;
}}


using namespace Framework::Hosts;


namespace Framework {
namespace Database{

	class DatabaseContextFactory {

	public:

		DatabaseContextFactory(Host* h);

		int Initialize();

		DatabaseContext* GetContext();

	protected:

		bool initialized = false;

		Host* host;

		DatabaseContext* dbContext;

		virtual int createDatabaseContext();

	};

}}





#endif