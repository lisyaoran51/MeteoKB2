#include "ConfigManager.h"


using namespace Framework::Configurations;
using namespace std;


template<typename T>
ConfigManager<T>::ConfigManager(): RegisterType("ConfigManager")
{
	// TODO: �Ӹ̨ӻ����ӬO�C���إ߮ɡA�j�����InitializeDefault�A���O�bc++�̫غc�l�������virtual�禡
	// �o�Ӱ��D�n��O����k�ѨM
	InitializeDefault();
}

template<typename T>
int ConfigManager<T>::Load()
{
	// �o�Ӧ��n������?
	// Ū����?
	return 0;
}

template<typename T>
int ConfigManager<T>::Set(T lookup, int value)
{
	configStoreInt[lookup] = value;
	return 0;
}

template<typename T>
int ConfigManager<T>::Set(T lookup, MTO_FLOAT value)
{
	configStoreFloat[lookup] = value;
	return 0;
}

template<typename T>
int ConfigManager<T>::Set(T lookup, bool value)
{
	configStoreBool[lookup] = value;
	return 0;
}

template<typename T>
int ConfigManager<T>::Set(T lookup, string value)
{
	configStoreString[lookup] = value;
	return 0;
}

/*
template<typename T>
template<typename U>
U ConfigManager<T>::Get(T lookup)
{
	if (typeid(U) == typeid(int))
	{
		map<int, Bar>::iterator iter = configStoreInt.find(lookup);
		if (iter != m.end())
		{
			// key 2 exists, do something with iter->second (the value)
		}
	}
		return configStoreInt[lookup];
	if (typeid(U) == typeid(MTO_FLOAT))
		return configStoreFloat[lookup];
	if (typeid(U) == typeid(bool))
		return configStoreBool[lookup];
	if (typeid(U) == typeid(string))
		return configStoreString[lookup];
	return NULL;
}*/

template<typename T>
template<typename U>
bool ConfigManager<T>::Get(T lookup, U * out)
{
	if (typeid(U) == typeid(int))
	{
		typename map<T, int>::iterator iter = configStoreInt.find(lookup);
		if (iter != configStoreInt.end())
		{
			*out = configStoreInt[lookup];
			return true;
		}
	}

	if (typeid(U) == typeid(MTO_FLOAT))
	{
		typename map<T, MTO_FLOAT>::iterator iter = configStoreFloat.find(lookup);
		if (iter != configStoreFloat.end())
		{
			*out = configStoreFloat[lookup];
			return true;
		}
	}

	if (typeid(U) == typeid(bool))
	{
		typename map<T, bool>::iterator iter = configStoreBool.find(lookup);
		if (iter != configStoreBool.end())
		{
			*out = configStoreBool[lookup];
			return true;
		}
	}

	if (typeid(U) == typeid(string))
	{
		typename map<T, string>::iterator iter = configStoreString.find(lookup);
		if (iter != configStoreString.end())
		{
			*out = configStoreString[lookup];
			return true;
		}
	}

	return false;
}
