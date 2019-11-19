#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include "../../Framework/Allocation/Hierachal/Container.h"
#include"../../Util/MtoType.h"
#include<string>
#include<map>


#define CONFIG_PATH 


using namespace Framework::Allocation::Hierachal;
using namespace std;


namespace Framework {
namespace Configurations {

	template<typename T>
	class ConfigManager: public MtoObject {

		typedef T Setting;

		map<Setting, int> configStoreInt;
		map<Setting, MTO_FLOAT> configStoreFloat;
		map<Setting, bool> configStoreBool;
		map<Setting, string> configStoreString;

		bool initialized = false;

	public:

		ConfigManager(): RegisterType("ConfigManager")
		{
			// TODO: �Ӹ̨ӻ����ӬO�C���إ߮ɡA�j�����InitializeDefault�A���O�bc++�̫غc�l�������virtual�禡
			// �o�Ӱ��D�n��O����k�ѨM
			//registerLoad(bind((int(ConfigManager<T>::*)())&ConfigManager<T>::load, this));
		}

		int Initialize() {
			// �o�Ӧ��n������?
			// Ū����?
			InitializeDefault();
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

		// �o�Ӽg�k����P�_�O�_����Ʀbmap��
		// template<typename U>
		// U Get(T lookup);

		/// <summary>
		/// �Ψ�get�]�w�����e�A�]��c++�����\���O���T�w���A�N������ƭȶi�h�A�ҥH��get����4�Ө��
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
		/// �Ψ�get�]�w�����e�A�]��c++�����\���O���T�w���A�N������ƭȶi�h�A�ҥH��get����4�Ө��
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
		/// �Ψ�get�]�w�����e�A�]��c++�����\���O���T�w���A�N������ƭȶi�h�A�ҥH��get����4�Ө��
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
		/// �Ψ�get�]�w�����e�A�]��c++�����\���O���T�w���A�N������ƭȶi�h�A�ҥH��get����4�Ө��
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