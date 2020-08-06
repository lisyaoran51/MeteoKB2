#ifndef DYNAMIC_PATH_RESOURCE_STORE_H
#define DYNAMIC_PATH_RESOURCE_STORE_H


#include <vector>
#include <iostream>
#include "ResourceStore.h"
#include <fstream>


using namespace std;


namespace Framework {
namespace IO {
namespace Stores {


	/// <summary>
	/// �p�����R�M���ɦW�A��A�ݭn�ɮת��ɭԡA�u�n�I�sGet�N�|�h���A�n���ɮסA�åBŪ��stream���A
	/// T���ɮ�Ū�X�Ӫ��Φ��A�i��Obyte array����
	/// </summary>
	class DynamicPathResourceStore: public ResourceStore<char*> {

	public:

		DynamicPathResourceStore(string p);

		virtual char* Get(string name);

		virtual fstream* GetStream(string name);

		virtual string GetFilePath(string name);

	protected:


	private:

		string path;

	};



}}}






#endif
