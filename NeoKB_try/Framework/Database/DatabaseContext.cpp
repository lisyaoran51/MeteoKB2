#include "DatabaseContext.h"


using namespace Framework::Database;




int DatabaseContext::EntityModel::RetrieveInt(void * entity, string attribute)
{
	if(onQueryInt.at(attribute) != nullptr)
		return onQueryInt.at(attribute)(entity);
	else throw invalid_argument("DatabaseContext::EntityModel::RetrieveInt(): no such attribute in this entity");
}

string DatabaseContext::EntityModel::RetrieveString(void * entity, string attribute)
{
	if (onQueryString.at(attribute) != nullptr)
		return onQueryString.at(attribute)(entity);
	else throw invalid_argument("DatabaseContext::EntityModel::RetrieveString(): no such attribute in this entity");
}

double DatabaseContext::EntityModel::RetrieveDouble(void * entity, string attribute)
{
	if (onQueryDouble.at(attribute) != nullptr)
		return onQueryDouble.at(attribute)(entity);
	else throw invalid_argument("DatabaseContext::EntityModel::RetrieveDouble(): no such attribute in this entity");
}

bool DatabaseContext::EntityModel::RetrieveBool(void * entity, string attribute)
{
	if (onQueryBool.at(attribute) != nullptr)
		return onQueryBool.at(attribute)(entity);
	else throw invalid_argument("DatabaseContext::EntityModel::RetrieveBool(): no such attribute in this entity");
}

int DatabaseContext::EntityModel::AddOnQueryInt(function<int(void*)> queryFunction, string attribute)
{
	onQueryInt[attribute] = queryFunction;
	return 0;
}

int DatabaseContext::EntityModel::AddOnQueryString(function<string(void*)> queryFunction, string attribute)
{
	onQueryString[attribute] = queryFunction;
	return 0;
}

int DatabaseContext::EntityModel::AddOnQueryDouble(function<double(void*)> queryFunction, string attribute)
{
	onQueryDouble[attribute] = queryFunction;
	return 0;
}

int DatabaseContext::EntityModel::AddOnQueryBool(function<bool(void*)> queryFunction, string attribute)
{
	onQueryBool[attribute] = queryFunction;
	return 0;
}

DatabaseContext::DatabaseContext(Storage * s)
{
	storage = s;
}

int DatabaseContext::Initialize() {

	createModel();
	prepare();
	initialized = true;
}

DatabaseSet<KeyBinding>* DatabaseContext::GetKeyBindings()
{
	if (!initialized)
		throw runtime_error("DatabaseContext::GetKeyBindings(): not initialized.");

	return &databasedKeyBinding;
}

DatabaseSet<FileInfo>* DatabaseContext::GetFileInfos()
{
	if (!initialized)
		throw runtime_error("DatabaseContext::GetFileInfos(): not initialized.");

	return &fileInfo;
}

DatabaseSet<RulesetInfo>* DatabaseContext::GetRulesetInfos()
{
	if (!initialized)
		throw runtime_error("DatabaseContext::GetRulesetInfos(): not initialized.");

	return &rulesetInfo;
}

DatabaseSet<SmInfo>* DatabaseContext::GetSmInfos()
{
	if (!initialized)
		throw runtime_error("DatabaseContext::GetSmInfos(): not initialized.");

	return &smInfo;
}

int DatabaseContext::prepare()
{
	// TODO: 與資料庫連結
	return 0;
}

int DatabaseContext::createModel()
{
	// need to be override
	return 0;
}
