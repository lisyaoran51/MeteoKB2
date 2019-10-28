#ifndef DATABASE_SET_H
#define DATABASE_SET_H


#include <map>
#include <vector>
#include <functional>
#include <string>
#include <utility>


using namespace std;



namespace Framework {
namespace Database {

	/// <summary>
	/// 一列資料
	/// </summary>
	template<typename TEntity2>
	struct Entity {
		TEntity2* Object;

		map<string, int> PropertiesInt;
		map<string, string> PropertiesString;
		map<string, double> PropertiesDouble;
		map<string, bool> PropertiesBool;
	};

	template<typename TObject>
	class DatabaseSet {

	protected:
		
		vector<TObject*> objects;

		vector<Entity<TObject>*> entitiesExtracted;

	public:

		int AddEntity(Entity<TObject>* entity) {
			entitiesExtracted.push_back(entity);
			entities.push_back(entity->Object);
			return 0;
		}

		/// <summary>
		/// 用完要刪掉，不然會memory leak
		/// </summary>
		vector<TObject*>* GetEntities() {
			vector<TObject*>* cloneObjects = new vector<TObject*>();
			cloneEntities->assign(objects.begin(), objects.end());
			return cloneEntities;
		}

		/// <summary>
		/// 用完要刪掉，不然會memory leak
		/// </summary>
		template<typename TAttribute>
		vector<TObject*>* GetEntitiesOfPropertiy(string attribute, TAttribute value) {

			
			if(typeid(TAttribute).name() == typeid(int).name())
				return GetEntitiesOfPropertiyInt(attribute, dynamic_cast<int>(value));

			if (typeid(TAttribute).name() == typeid(string).name())
				return GetEntitiesOfPropertiyString(attribute, dynamic_cast<string>(value));

			if (typeid(TAttribute).name() == typeid(char*).name())
				return GetEntitiesOfPropertiyString(attribute, dynamic_cast<char*>(value));

			if (typeid(TAttribute).name() == typeid(double).name())
				return GetEntitiesOfPropertiyDouble(attribute, dynamic_cast<double>(value));

			if (typeid(TAttribute).name() == typeid(bool).name())
				return GetEntitiesOfPropertiyBool(attribute, dynamic_cast<bool>(value));

			throw invalid_argument("vector<TObject*>* GetEntitiesOfPropertiy(): the template type is wrong.")

			return nullptr;
		}

		/// <summary>
		/// 用完要刪掉，不然會memory leak
		/// </summary>
		vector<TObject*>* GetEntitiesOfPropertiyInt(string attribute, int value) {
			vector<TObject*>* cloneEntities = new vector<TObject*>();

			for (int i = 0; i < entitiesExtracted.size(); i++) {
				if (entitiesExtracted[i]->PropertiesInt.at(attribute) == value)
					cloneEntities->push_back(entitiesExtracted[i]->Object);
			}

			return cloneEntities;
		}

		/// <summary>
		/// 用完要刪掉，不然會memory leak
		/// </summary>
		vector<TObject*>* GetEntitiesOfPropertiyString(string attribute, string value) {
			vector<TObject*>* cloneEntities = new vector<TObject*>();

			for (int i = 0; i < entitiesExtracted.size(); i++) {
				if (entitiesExtracted[i]->PropertiesString.at(attribute) == value)
					cloneEntities->push_back(entitiesExtracted[i]->Object);
			}

			return cloneEntities;
		}

		/// <summary>
		/// 用完要刪掉，不然會memory leak
		/// </summary>
		vector<TObject*>* GetEntitiesOfPropertiyDouble(string attribute, double value) {
			vector<TObject*>* cloneEntities = new vector<TObject*>();

			for (int i = 0; i < entitiesExtracted.size(); i++) {
				if (entitiesExtracted[i]->PropertiesDouble.at(attribute) == value)
					cloneEntities->push_back(entitiesExtracted[i]->Object);
			}

			return cloneEntities;
		}
		
		/// <summary>
		/// 用完要刪掉，不然會memory leak
		/// </summary>
		vector<TObject*>* GetEntitiesOfPropertiyBool(string attribute, bool value) {
			vector<TObject*>* cloneEntities = new vector<TObject*>();

			for (int i = 0; i < entitiesExtracted.size(); i++) {
				if (entitiesExtracted[i]->PropertiesBool.at(attribute) == value)
					cloneEntities->push_back(entitiesExtracted[i]->Object);
			}

			return cloneEntities;
		}

	protected:



		
	};


}}






#endif