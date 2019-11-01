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

int DatabaseContext::EntityModel::AddOnQueryInt(function<int(void* entity)> queryFunction, string attribute)
{
	onQueryInt[attribute] = queryFunction;
	return 0;
}

int DatabaseContext::EntityModel::AddOnQueryString(function<string(void* entity)> queryFunction, string attribute)
{
	onQueryString[attribute] = queryFunction;
	return 0;
}

int DatabaseContext::EntityModel::AddOnQueryDouble(function<double(void* entity)> queryFunction, string attribute)
{
	onQueryDouble[attribute] = queryFunction;
	return 0;
}

int DatabaseContext::EntityModel::AddOnQueryBool(function<bool(void* entity)> queryFunction, string attribute)
{
	onQueryBool[attribute] = queryFunction;
	return 0;
}

DatabaseSet<KeyBinding>* DatabaseContext::GetKeyBindings()
{
	return &databasedKeyBinding;
}

DatabaseSet<FileInfo>* DatabaseContext::GetFileInfos()
{
	return &fileInfo;
}

DatabaseSet<RulesetInfo>* DatabaseContext::GetRulesetInfos()
{
	return &rulesetInfo;
}

DatabaseSet<SmInfo>* DatabaseContext::GetSmInfos()
{
	return &smInfo;
}

int DatabaseContext::createModel()
{
	// need to be override
	return 0;
}
