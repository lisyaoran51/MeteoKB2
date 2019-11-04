#include "MeteoDatabaseContext.h"



using namespace Framework::Database;




string MeteoDatabaseContext::smInfoFilenameQueryFunction(void * object)
{
	SmInfo* s = static_cast<SmInfo*>(object);

	return s->fileName;
}

MeteoDatabaseContext::MeteoDatabaseContext(Storage * s): DatabaseContext(s)
{
}

int MeteoDatabaseContext::createModel()
{
	EntityModel* smQuerier = new EntityModel();
	smQuerier->AddOnQueryString(bind(&MeteoDatabaseContext::smInfoFilenameQueryFunction, this, placeholders::_1), "FileName");

	return 0;
}
