#ifndef METEO_DATABASE_CONTEXT_H
#define METEO_DATABASE_CONTEXT_H

#include "DatabaseContext.h"

namespace Framework {
namespace Database{

	class MeteoDatabaseContext : public DatabaseContext {

	protected:

		virtual int createModel();

	};

}}




#endif