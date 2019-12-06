#ifndef RESOURCE_STORE_H
#define RESOURCE_STORE_H


#include <vector>
#include <iostream>
#include <string>
#include "../FileSystemInterface.h"
#include "../../Allocation/Hierachal/MtoObject.h"
#include "../FileNotFoundException.h"

using namespace std;
using namespace Framework::IO;
using namespace Framework::Allocation::Hierachal;


namespace Framework {
namespace IO {
namespace Stores {


	/// <summary>
	/// �p�����R�M���ɦW�A��A�ݭn�ɮת��ɭԡA�u�n�I�sGet�N�|�h���A�n���ɮסA�åBŪ��stream���A
	/// T���ɮ�Ū�X�Ӫ��Φ��A�i��Obyte array����
	/// ���GT���ӥΤ���
	/// </summary>
	template<typename T>
	class ResourceStore : public MtoObject{

	public:

		ResourceStore(): RegisterType("ResourceStore") {

		}


		virtual T Get(string name) = 0;

		virtual ifstream* GetStream(string name) = 0;
		
		virtual string GetFilePath(string name) = 0;

	};



}}}






#endif
