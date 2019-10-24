#ifndef PLATFORM_STORAGE_H
#define PLATFORM_STORAGE_H


#include "Storage.h"



namespace Framework {
namespace IO{


	/// <summary>
	/// 再各平台可以用的storage，目前盡量用c++內建的lib寫完，locateBasePath目前只有linux可以用
	/// </summary>
	class PlatformStorage : public Storage {


	public:

		PlatformStorage(string bName);

		/// <summary>
		/// 查看有沒有這個檔案
		/// </summary>
		virtual bool Exist(string filePath);

		/// <summary>
		/// 查看有沒有這個資料夾
		/// </summary>
		virtual bool ExistDirectory(string directoryPath);

		/// <summary>
		/// 刪檔
		/// </summary>
		virtual int Delete(string filePath);

		/// <summary>
		/// 刪資料夾和下面所有東西
		/// </summary>
		virtual int DeleteDirectory(string directoryPath);

		/// <summary>
		/// 找出所有資料夾
		/// </summary>
		virtual vector<string>* GetDirectories(string directoryPath);

		virtual ifstream GetStream(string filePath);

	protected:

		virtual string locateBasePath();

		virtual Storage* clone(string subDirectory);


	};


}}







#endif