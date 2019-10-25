#ifndef STORAGE_H
#define STORAGE_H


#include <string>
#include <vector>
#include <fstream>


using namespace std;



namespace Framework {
namespace IO{


	class Storage {


	public:
		
		/// <summary>
		/// 把輸入路徑和strorage的路徑串在一起回傳，只能輸入輸入資料夾路徑
		/// 要記得開頭不能有/，會出錯
		/// </summary>
		string GetUsableDirectoryPathFor(string directoryPath, bool createIfNotExist = false);

		/// <summary>
		/// 把輸入路徑和strorage的路徑串在一起回傳，只能輸入輸入檔案路徑
		/// 要記得開頭不能有/，會出錯
		/// </summary>
		string GetUsableFilePathFor(string directoryPath, bool createIfNotExist = false);

		/// <summary>
		/// 查看有沒有這個檔案
		/// </summary>
		virtual bool Exist(string filePath) = 0;

		/// <summary>
		/// 查看有沒有這個資料夾
		/// </summary>
		virtual bool ExistDirectory(string directoryPath) = 0;

		/// <summary>
		/// 刪檔
		/// </summary>
		virtual int Delete(string filePath) = 0;

		/// <summary>
		/// 刪資料夾和下面所有東西
		/// </summary>
		virtual int DeleteDirectory(string directoryPath) = 0;

		/// <summary>
		/// 找出所有資料夾
		/// </summary>
		virtual vector<string>* GetDirectories(string directoryPath) = 0;

		Storage* GetStorageForDirectory(string directoryPath);

		/// <summary>
		/// 使用前要先check exist，不然會出錯
		/// </summary>
		virtual ifstream* GetStream(string filePath) = 0;

	protected:


		/// <summary>
		/// 從系統預設的路徑上面再指定一個資料夾 前後都不能有斜線
		/// </summary>
		string baseName;

		/// <summary>
		/// 從系統預設的路徑 前後都不能有斜線
		/// </summary>
		string basePath;

		Storage(string bName, string sDirectory = "");

		/// <summary>
		/// 前後都不能有斜線
		/// </summary>
		string subDirectory;

		virtual string locateBasePath() = 0;

		/// <summary>
		/// 前後都不能有斜線，會現在有的subdirectory下面再嘉一個subDirectory
		/// </summary>
		virtual Storage* clone(string newSubDirectory = "") = 0;


	};


}}





#endif