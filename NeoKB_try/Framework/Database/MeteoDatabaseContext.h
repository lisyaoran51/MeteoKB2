#ifndef METEO_DATABASE_CONTEXT_H
#define METEO_DATABASE_CONTEXT_H

#include "DatabaseContext.h"
#include "../IO/Storage.h"

using namespace Framework::IO;

namespace Framework {
namespace Database{

	class MeteoDatabaseContext : public DatabaseContext {

	public:

		MeteoDatabaseContext(Storage* s);

	protected:

		virtual int createModel();

	};

}}




#endif