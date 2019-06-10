#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include"../../Util/Hierachal/Loadable.h"
#include"../../Util/MtoType.h"
#include<string>
#include<map>


#define CONFIG_PATH 


using namespace Util::Hierachal;
using namespace std;


namespace Base {
namespace Config{

	template<typename T>
	class ConfigManager: public Loadable {

		typedef T Setting;

		map<Setting, int> configStoreInt;
		map<Setting, MTO_FLOAT> configStoreFloat;
		map<Setting, bool> configStoreBool;
		map<Setting, string> configStoreString;

		int load() {
			// 這個式要做什麼?
			// 讀文件嗎?
			InitializeDefault();
			return 0;
		}

	public:

		ConfigManager(): Loadable(), RegisterType("ConfigManager")
		{
			// TODO: 照裡來說應該是每次建立時，強制執行InitializeDefault，但是在c++裡建構子不能執行virtual函式
			// 這個問題要找別的方法解決
			registerLoad(bind((int(ConfigManager<T>::*)())&ConfigManager<T>::load, this));
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

		virtual int InitializeDefault() = 0;

	};

}}



#endif