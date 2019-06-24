#ifndef RESOURCE_STORE_H
#define RESOURCE_STORE_H


#include <vector>
#include <iostream>


using namespace std;


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

		ResourceStore(ResourceStore* store);

		ResourceStore(vector<ResourceStore*>* ss);

		T Get(string name);

		ifstream* GetStream(string name);

		int AddExtension(string extension);

	protected:

		vector<string>* getFileNames();

	private:

		vector<ResourceStore*> stores;

		vector<string> extensions;

	};




}}}






#endif
