#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "../../Framework/Allocation/Hierachal/MtoObject.h"
#include "../../Util/MtoType.h"
#include <string>
#include <map>
#include "../IO/Storage.h"
#include "../../Util/StringSplitter.h"


#define CONFIG_PATH 


using namespace Framework::Allocation::Hierachal;
using namespace std;
using namespace Framework::IO;
using namespace Util;


namespace Framework {
namespace Configurations {

	class ConfigManager : public MtoObject {

	public:

		ConfigManager();

		virtual int Initialize(Storage* s) = 0;

	protected:

		string fileName = "";

		Storage* storage = nullptr;
		
	};

	
	template<typename T>
	class TConfigManager : public ConfigManager {

		typedef T Setting;

		map<Setting, int> configStoreInt;
		map<Setting, MTO_FLOAT> configStoreFloat;
		map<Setting, bool> configStoreBool;
		map<Setting, string> configStoreString;

		bool initialized = false;

	public:

		TConfigManager() : RegisterType("TConfigManager"), ConfigManager()
		{
			// TODO: 照裡來說應該是每次建立時，強制執行InitializeDefault，但是在c++裡建構子不能執行virtual函式
			// 這個問題要找別的方法解決
			//registerLoad(bind((int(TConfigManager<T>::*)())&TConfigManager<T>::load, this));
		}

		virtual int Initialize(Storage* s) {
			storage = s;

			setNamesOfConfig();
			initializeDefault();

			loadConfigs();
			initialized = true;
			return 0;
		}

		int Set(T lookup, int value) {
			configStoreInt[lookup] = value;
			return 0;
		}

		int Set(T lookup, MTO_FLOAT value) {
			configStoreFloat[lookup] = value;
			return 0;
		}

		int Set(T lookup, bool value) {
			configStoreBool[lookup] = value;
			return 0;
		}

		int Set(T lookup, string value) {
			configStoreString[lookup] = value;
			return 0;
		}

		// 這個寫法不能判斷是否有資料在map裡
		// template<typename U>
		// U Get(T lookup);

		/// <summary>
		/// 用來get設定的內容，因為c++不允許不是先確定型態就直接塞數值進去，所以把get分成4個函示
		/// </summary>
		bool Get(T lookup, int* out) {

			typename map<Setting, int>::iterator iter = configStoreInt.find(lookup);
			if (iter != configStoreInt.end())
			{
				*out = configStoreInt[lookup];
				return true;
			}
			return false;
		}

		/// <summary>
		/// 用來get設定的內容，因為c++不允許不是先確定型態就直接塞數值進去，所以把get分成4個函示
		/// </summary>
		bool Get(T lookup, MTO_FLOAT* out) {

			typename map<Setting, MTO_FLOAT>::iterator iter = configStoreFloat.find(lookup);
			if (iter != configStoreFloat.end())
			{
				*out = configStoreFloat[lookup];
				return true;
			}
			return false;
		}

		/// <summary>
		/// 用來get設定的內容，因為c++不允許不是先確定型態就直接塞數值進去，所以把get分成4個函示
		/// </summary>
		bool Get(T lookup, bool* out) {

			typename map<Setting, bool>::iterator iter = configStoreBool.find(lookup);
			if (iter != configStoreBool.end())
			{
				*out = configStoreBool[lookup];
				return true;
			}
			return false;
		}

		/// <summary>
		/// 用來get設定的內容，因為c++不允許不是先確定型態就直接塞數值進去，所以把get分成4個函示
		/// </summary>
		bool Get(T lookup, string* out) {

			typename map<Setting, string>::iterator iter = configStoreString.find(lookup);
			if (iter != configStoreString.end())
			{
				*out = configStoreString[lookup];
				return true;
			}
			return false;
		}



	protected:

		map<string, Setting> namesOfConfig;

		virtual int initializeDefault() = 0;

		enum class ConfigType {
			None,
			Int,
			String,
			Float,
			Bool
		};

		ConfigType checkConfigType(string s) {
			
			if (s == "")
				return ConfigType::None;

			if (s == "true" || s == "false")
				return ConfigType::Bool;

			string::const_iterator it = s.begin();
			while (it != s.end() && isdigit(*it)) ++it;
			if (it == s.end())
				return ConfigType::Int;

			it = s.begin();
			while (it != s.end() && (isdigit(*it) || *it == '.')) ++it;
			if (it+1 == s.end() - 1 && *(it+1) == 'f')
				return ConfigType::Float;

			it = s.begin();
			while (it != s.end() && (
				((int)(*it) >= 48 && (int)(*it) <= 57) ||	// 0~9
				((int)(*it) >= 65 && (int)(*it) <= 90) ||	// a~z
				((int)(*it) >= 97 && (int)(*it) <= 122) ||	// A~Z
				(int)(*it) == 95))							// _
				++it;
			if (it == s.end())
				return ConfigType::String;

			return ConfigType::None;

		}

		virtual int loadConfigs() {

			if (fileName == "")
				return -1;

			if (storage == nullptr)
				return -1;

			fstream* stream = storage->GetStream(fileName);

			if (stream == nullptr)
				return -1;

			do {

				getline(*stream, line);
				LOG(LogLevel::Depricated) << "TConfigManager::loadConfigs() : read line [" << line << "].";

			} while (!stream->eof() && line.empty());

			while (!stream->eof()) {
				getline(*stream, line);

				LOG(LogLevel::Finer) << "int TConfigManager::loadConfigs() : read line [" << line << "].";

				if (line == " " || line.empty())
					continue;

				if (line.find("//") == 0)
					continue;

				if (line.find("#") == 0)
					continue;

				vector<string> pair = StringSplitter::Split(line, ":");

				if (pair.size() != 2)
					continue;

				string key = StringSplitter::Trim(pair[0]);
				string value = StringSplitter::Trim(pair[1]);

				if (key == "" || value == "")
					continue;

				if (namesOfConfig.find(key) == namesOfConfig.end())
					continue;

				switch (checkConfigType(value)) {
				case ConfigType::None:
					continue;
					break;
				case ConfigType::Bool:
					if (value == "true")
						Set(namesOfConfig[key], true);
					else
						Set(namesOfConfig[key], false);
					break;
				case ConfigType::Int:
					Set(namesOfConfig[key], stoi(value));
					break;
				case ConfigType::Float:
					Set(namesOfConfig[key], (MTO_FLOAT)stof(value));
					break;
				case ConfigType::String:
					Set(namesOfConfig[key], value);
					break;

				}
			}

			return 0;

		}

	};

}}



#endif