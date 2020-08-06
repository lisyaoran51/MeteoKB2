#ifndef FAKE_METEO_DATABASE_CONTEXT_H
#define FAKE_METEO_DATABASE_CONTEXT_H


#include "MeteoDatabaseContext.h"




namespace Framework {
namespace Database{

	class FakeMeteoDatabaseContext : public MeteoDatabaseContext {
		
		vector<SmInfo*>* parseSmInfoCsv(fstream* stream);

	public:

		FakeMeteoDatabaseContext(Storage* s);

	protected:

		int prepare();

	};

}}






#endif