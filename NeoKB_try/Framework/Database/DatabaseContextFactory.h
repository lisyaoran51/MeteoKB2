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

		GameHost* host;

		DatabaseContext* dbContext;

	public:

		DatabaseContextFactory(GameHost* h);

		DatabaseContext* GetContext();

	};

}}





#endif