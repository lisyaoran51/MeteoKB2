#ifndef RESOURCE_STORE_H
#define RESOURCE_STORE_H


#include <vector>
#include <iostream>
#include "../FileSystemInterface.h"

using namespace std;
using namespace Framework::IO;



namespace Framework {
namespace IO {
namespace Stores {


	/// <summary>
	/// �p�����R�M���ɦW�A��A�ݭn�ɮת��ɭԡA�u�n�I�sGet�N�|�h���A�n���ɮסA�åBŪ��stream���A
	/// T���ɮ�Ū�X�Ӫ��Φ��A�i��Obyte array����
	/// </summary>
	template<typename T>
	class ResourceStore {

	public:

		ResourceStore(ResourceStore<T>* store = nullptr);

		ResourceStore(vector<ResourceStore<T>*>* ss);

		virtual T Get(string name);

		virtual ifstream* GetStream(string name);

		int AddExtension(string extension);

		int AddStore(ResourceStore<T>* store);

	protected:

		vector<string>* getFileNames(string name);

		/// <summary>
		/// �Ψ��˯���Ӹ�Ƨ��U�������ɮ�
		/// </summary>
		static FileSystemInterface* fileSystemInterface;

		/// <summary>
		/// �Ψ��˯���Ӹ�Ƨ��U�������ɮ�
		/// </summary>
		virtual int initializeFileSystemInterface();

	private:

		vector<ResourceStore<T>*> stores;

		vector<string> extensions;

		

	};

	struct FileNotFoundException : public runtime_error {

		FileNotFoundException(char* message): runtime_error(message) {}

		const char * what() const throw () {
			return runtime_error::what();
		}
	};


}}}






#endif
