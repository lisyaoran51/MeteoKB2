#ifndef SECOND_PERIOD_H
#define SECOND_PERIOD_H

#include <vector>
#include <map>
#include <functional>

using namespace std;

namespace Util {
namespace DataStructure {

	template<typename T>
	class SecondPeriod {


	public:

		SecondPeriod(int sSecond) {
			startSecond = sSecond;
		}

		int startSecond = -1;

		vector<T> objects;

		map<string, vector<T>*> classifiedObjects;

		bool classified = false;

		int Add(T o) {
			objects.push_back(o);
			if (classified) {
				// throw error: 已經分類好就不要再加了，新加的物件沒被分類
				// TODO: 不然就是要留著分類的function，每次加新的物件就可以被分類一次，不過我們程式應該不用寫那麼複雜
			}
			return objects.size();
		}

		/// <summary>
		/// 用function去分類，提早分好類可以加速遊戲中搜尋物件的速度
		/// </summary>
		int Classify(string typeName, function<bool(T)> checkIsInClass) {
			vector<T>* newClassOfObjects = new vector<T>();

			for (int i = 0; i < objects.size(); i++) {
				if (checkIsInClass(objects[i])) {
					newClassOfObjects->push_back(objects[i]);
				}
			}

			if (classifiedObjects.find(typeName) != classifiedObjects.end()) {
				delete classifiedObjects[typeName];
			}

			classifiedObjects[typeName] = newClassOfObjects;

			return newClassOfObjects->size();
		}

		/// <summary>
		/// 用type去分類，提早分好類可以加速遊戲中搜尋物件的速度
		/// </summary>
		template<typename T2>
		int Classify(string typeName) {
			vector<T>* newClassOfObjects = new vector<T>();

			for (int i = 0; i < objects.size(); i++) {
				if (dynamic_cast<T2>(objects[i]) != nullptr) {
					newClassOfObjects->push_back(objects[i]);
				}
			}

			if (classifiedObjects.find(typeName) != classifiedObjects.end()) {
				delete classifiedObjects[typeName];
			}

			classifiedObjects[typeName] = newClassOfObjects;

			return newClassOfObjects->size();
		}
	};

}
}


#endif