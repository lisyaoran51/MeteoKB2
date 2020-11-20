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

		PlatformStorage(string bName, string sDirectory = "");

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
		/// 找出所有資料夾，用完要把vector回收
		/// </summary>
		virtual vector<string>* GetDirectories(string directoryPath);

		virtual fstream* GetStream(string filePath);

		virtual fstream* GetStream(string filePath, bool volitile, bool binary = false);

		/// <summary>
		/// 會自動建資料夾和檔案
		/// 寫入專用，平常不要用這個
		/// </summary>
		virtual fstream* GetStream(string filePath, FileAccess fileAccess, FileMode fileMode, bool binary = false);


	protected:

		virtual string locateBasePath();

		virtual string locateWriteBasePath();

		virtual Storage* clone(string newSubDirectory = "");


	};


}}







#endif