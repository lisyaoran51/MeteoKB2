#ifndef DATABASE_CONTEXT_H
#define DATABASE_CONTEXT_H

#include "DatabaseSet.h"
#include "../Input/KeyBindings/KeyBinding.h"
#include "../../Games/Ruleset/RulesetInfo.h"
#include "../IO/FileInfo.h"
#include "../../Games/Sheetmusic/SheetmusicInfo.h"
#include "../IO/Storage.h"

// 用法:
// GetDbSet<RulesetInfo>()->GetEntitiesOfPropertiy("name")
// GetDbSet<RulesetInfo>()->GetEntitiesOfPropertiy("name")->at(0)->RetrieveInt("ID"); 
// RetrieveInt(GetDbSet<RulesetInfo>()->GetEntitiesOfPropertiy("name")->at(0), "ID")


using namespace Framework::Input::KeyBindings;
using namespace Games::Rulesets;
using namespace Framework::IO;
using namespace Games::Sheetmusics;
using namespace Framework::IO;


namespace Framework {
namespace Database{

	/// <summary>
	/// 資料庫內容
	/// </summary>
	class DatabaseContext {


	public:

		DatabaseContext(Storage* s);

		template<typename TObject>
		int AddDbSetEntity(TObject* object);

		template<typename TEntity>
		DatabaseSet<TEntity>* GetDbSet();

		/// <summary>
		/// 沒用
		/// </summary>
		DatabaseSet<KeyBinding>* GetKeyBindings();

		/// <summary>
		/// 沒用
		/// </summary>
		DatabaseSet<FileInfo>* GetFileInfos();

		DatabaseSet<RulesetInfo>* GetRulesetInfos();

		DatabaseSet<SmInfo>* GetSmInfos();

		template<typename TEntity>
		int RetrieveInt(TEntity* entity, string attribute);
		
		template<typename TEntity>
		string RetrieveString(TEntity* entity, string attribute);
		
		template<typename TEntity>
		double RetrieveDouble(TEntity* entity, string attribute);
		
		template<typename TEntity>
		bool RetrieveBool(TEntity* entity, string attribute);



		
	protected:

		virtual int prepare();

		Storage* storage;

		//DatabaseSet<SmDifficulty>* BeatmapDifficulty;
		//DatabaseSet<SmMetadata>* BeatmapMetadata;
		//DatabaseSet<SmSetInfo>* BeatmapSetInfo;
		DatabaseSet<KeyBinding> databasedKeyBinding;
		DatabaseSet<FileInfo> fileInfo;
		DatabaseSet<RulesetInfo> rulesetInfo;
		DatabaseSet<SmInfo> smInfo;



		class EntityModel {

		public:

			template<typename TAttribute>
			TAttribute Retrieve(void* entity, string attribute) {

				if (typeid(TAttribute).name() == typeid(int).name())
					return dynamic_cast<TAttribute>(RetrieveInt(entity, attribute));

				if (typeid(TAttribute).name() == typeid(string).name() ||
					typeid(TAttribute).name() == typeid(char*).name())
					return dynamic_cast<TAttribute>(RetrieveString(entity, attribute));

				if (typeid(TAttribute).name() == typeid(double).name())
					return dynamic_cast<TAttribute>(RetrieveDouble(entity, attribute));

				if (typeid(TAttribute).name() == typeid(bool).name())
					return dynamic_cast<TAttribute>(RetrieveBool(entity, attribute));

				throw invalid_argument("DatabaseSet<TEntity>* GetDbSet(): wrong template.");

				return TAttribute();
			}

			template<typename TEntity>
			Entity<TEntity>* BuildEntity(TEntity* e) {
				Entity<TEntity>* entity = new Entity<TEntity>();

				entity->Object = e;

				for (auto const& p : onQueryInt)
					entity->PropertiesInt.insert(pair<string, int>(p.first, p.second(e)));

				for (auto const& p : onQueryString)
					entity->PropertiesString.insert(pair<string, string>(p.first, p.second(e)));

				for (auto const& p : onQueryDouble)
					entity->PropertiesDouble.insert(pair<string, double>(p.first, p.second(e)));

				for (auto const& p : onQueryBool)
					entity->PropertiesBool.insert(pair<string, bool>(p.first, p.second(e)));

				return entity;
			}

			int RetrieveInt(void* entity, string attribute);
			string RetrieveString(void* entity, string attribute);
			double RetrieveDouble(void* entity, string attribute);
			bool RetrieveBool(void* entity, string attribute);

			int AddOnQueryInt(function<int(void*)> queryFunction, string attribute);
			int AddOnQueryString(function<string(void*)> queryFunction, string attribute);
			int AddOnQueryDouble(function<double(void*)> queryFunction, string attribute);
			int AddOnQueryBool(function<bool(void*)> queryFunction, string attribute);
		protected:

			map<string, function<int(void* entity)>> onQueryInt;
			map<string, function<string(void* entity)>> onQueryString;
			map<string, function<double(void* entity)>> onQueryDouble;
			map<string, function<bool(void* entity)>> onQueryBool;

		};

		/// <summary>
		/// char*是 typeid(T).name();
		/// </summary>
		map<char*, EntityModel*> querier;

		virtual int createModel();

	};

	template<typename TObject>
	int DatabaseContext::AddDbSetEntity(TObject * object)
	{

		DatabaseSet<TObject>* dbSet = GetDbSet<TObject>();

		Entity<TObject>* entity = querier.at((char*)typeid(TObject).name())->BuildEntity<TObject>(object);

		dbSet->AddEntity(entity);

		return 0;
	}

	template<typename TEntity>
	DatabaseSet<TEntity>* DatabaseContext::GetDbSet()
	{

		if (typeid(TEntity).name() == typeid(KeyBinding).name())
			return dynamic_cast<DatabaseSet<TEntity>*>(&databasedKeyBinding);

		if (typeid(TEntity).name() == typeid(FileInfo).name())
			return dynamic_cast<DatabaseSet<TEntity>*>(&fileInfo);

		if (typeid(TEntity).name() == typeid(RulesetInfo).name())
			return dynamic_cast<DatabaseSet<TEntity>*>(&rulesetInfo);

		throw invalid_argument("DatabaseSet<TEntity>* GetDbSet(): wrong template.");

		return nullptr;

	}

	template<typename TEntity>
	int DatabaseContext::RetrieveInt(TEntity * entity, string attribute)
	{
		return querier.at((char*)typeid(TEntity).name())->RetrieveInt(entity, attribute);
	}

	template<typename TEntity>
	string DatabaseContext::RetrieveString(TEntity * entity, string attribute)
	{
		return querier.at((char*)typeid(TEntity).name())->RetrieveString(entity, attribute);
	}

	template<typename TEntity>
	double DatabaseContext::RetrieveDouble(TEntity * entity, string attribute)
	{
		return querier.at((char*)typeid(TEntity).name())->RetrieveDouble(entity, attribute);
	}

	template<typename TEntity>
	bool DatabaseContext::RetrieveBool(TEntity * entity, string attribute)
	{
		return querier.at((char*)typeid(TEntity).name())->RetrieveBool(entity, attribute);
	}


}}






#endif