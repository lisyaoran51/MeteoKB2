#ifndef COMPOSITE_RESOURCE_STORE_H
#define COMPOSITE_RESOURCE_STORE_H


#include <vector>
#include <iostream>
#include <string>
#include "ResourceStore.h"

using namespace std;
using namespace Framework::IO;


namespace Framework {
namespace IO {
namespace Stores {


	/// <summary>
	/// �p�����R�M���ɦW�A��A�ݭn�ɮת��ɭԡA�u�n�I�sGet�N�|�h���A�n���ɮסA�åBŪ��stream���A
	/// T���ɮ�Ū�X�Ӫ��Φ��A�i��Obyte array����
	/// ���GT���ӥΤ���
	/// </summary>
	template<typename T>
	class CompositeResourceStore : public ResourceStore<T> {

	public:

		CompositeResourceStore(ResourceStore<T>* store = nullptr): RegisterType("ResourceStore") {


			if (store != nullptr)
				stores.push_back(store);

		}

		
		CompositeResourceStore(vector<ResourceStore<T>*>* ss){

			if (ss == nullptr)
				return;
			for (int i = 0; i < ss->size(); i++) {
				stores.push_back(ss->at(i));
			}
		}

		virtual T Get(string name) {
			vector<string>* names = getFileNames(name);

			for (int i = 0; i < stores.size(); i++) {
				for (int j = 0; j < names->size(); j++) {
					try {
						T temp = stores[i]->Get(names->at(j));
						delete names;
						return temp;
					}
					catch (FileNotFoundException& e) {
						continue;
					}
				}
			}
			string message = string("CompositeResourceStore::Get : cannot found \"") + name + string("\"");

			delete names;

			throw FileNotFoundException((char*)message.c_str());

			return T();
		}

		virtual ifstream* GetStream(string name) {
			vector<string>* names = getFileNames(name);

			ifstream * tempStream = nullptr;

			for (int i = 0; i < stores.size(); i++) {
				for (int j = 0; j < names->size(); j++) {

					tempStream = stores[i]->GetStream(names->at(j));

					if (tempStream != nullptr) {
						delete names;
						return tempStream;
					}
				}
			}

			delete names;

			return nullptr;
		}

		virtual string GetFilePath(string name) {
			vector<string>* names = getFileNames(name);

			string returnValue = "";

			for (int i = 0; i < stores.size(); i++) {
				for (int j = 0; j < names->size(); j++) {

					try {
						returnValue = stores[i]->GetFilePath(names->at(j));
						delete names;
						return returnValue;
					}
					catch(){}
				}
			}
			delete names;
			return returnValue;
		}

		int AddExtension(string extension) {
			extensions.push_back(extension);
			return 0;
		}

		CompositeResourceStore<T>* AddStore(ResourceStore<T>* store) {

			stores.push_back(store);
			return this;
		}

	protected:

		virtual vector<string>* getFileNames(string name) {
			vector<string>* names = nullptr;
			if (name.find(".") != name.npos)
				return new vector<string>{ name };

			names = new vector<string>{ name };
			for (int i = 0; i < extensions.size(); i++) {
				names->push_back(name + string(".") + extensions[i]);
			}

			return names;
		}

		/* ���O���ɮת����k�n�b�O���a��ΡA���O�b�o���
		/// <summary>
		/// �Ψ��˯���Ӹ�Ƨ��U�������ɮ�
		/// </summary>
		static FileSystemInterface* fileSystemInterface;

		/// <summary>
		/// �Ψ��˯���Ӹ�Ƨ��U�������ɮ�
		/// </summary>
		virtual int initializeFileSystemInterface() {

			if (fileSystemInterface == nullptr)
				initializeFileSystemInterface = new LinuxFileSystem();
			return 0;
		}
		*/
	private:

		vector<ResourceStore<T>*> stores;

		vector<string> extensions;

		

	};



}}}






#endif
