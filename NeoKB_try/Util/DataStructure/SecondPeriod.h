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
				// throw error: �w�g�����n�N���n�A�[�F�A�s�[������S�Q����
				// TODO: ���M�N�O�n�d�ۤ�����function�A�C���[�s������N�i�H�Q�����@���A���L�ڭ̵{�����Ӥ��μg�������
			}
			return objects.size();
		}

		/// <summary>
		/// ��function�h�����A�������n���i�H�[�t�C�����j�M���󪺳t��
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
		/// ��type�h�����A�������n���i�H�[�t�C�����j�M���󪺳t��
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