#include "PlatformStorage.h"

#include <experimental/filesystem>
#include <stdexcept>
#include "../../Util/Log.h"

/*
 * filesystem用法 (跟boost一樣)
 * https://kheresy.wordpress.com/2010/10/25/boost_filesystem/
 * https://en.cppreference.com/w/cpp/experimental/fs/is_directory
 * https://stackoverflow.com/questions/4980815/c-determining-if-directory-not-a-file-exists-in-linux
 * https://stackoverflow.com/questions/675039/how-can-i-create-directory-tree-in-c-linux
 */



using namespace Framework::IO;
using namespace std::literals::string_literals;
using namespace std::experimental::filesystem;
using namespace Util;



PlatformStorage::PlatformStorage(string bName, string sDirectory): Storage(bName, sDirectory), RegisterType("PlatformStorage")
{
	

}

bool PlatformStorage::Exist(string filePath)
{
	if(!initialized)
		throw runtime_error("int PlatformStorage::Exist() : Not initialized.");

	string usablePath = GetUsableFilePathFor(filePath);
	path p = usablePath;

	return exists(p);
}

bool PlatformStorage::ExistDirectory(string directoryPath)
{
	string usablePath = GetUsableDirectoryPathFor(directoryPath);
	path p = usablePath;

	return exists(p);
}

int PlatformStorage::Delete(string filePath)
{
	if (!initialized)
		throw runtime_error("int PlatformStorage::Delete() : Not initialized.");

	if (Exist(filePath)) {
		if (remove(GetUsableFilePathFor(filePath))) {
			return 0;
		}
		else {
			throw invalid_argument("int PlatformStorage::Delete() : The file is not deletable now.");
		}
	}

	return -1;
}

int PlatformStorage::DeleteDirectory(string directoryPath)
{
	if (!initialized)
		throw runtime_error("int PlatformStorage::DeleteDirectory() : Not initialized.");

	if (ExistDirectory(directoryPath)) {
		if (remove_all(GetUsableDirectoryPathFor(directoryPath))) {
			return 0;
		}
		else {
			throw invalid_argument("int PlatformStorage::DeleteDirectory() : The directory is not deletable now.");
		}
	}

	return -1;
}

vector<string>* PlatformStorage::GetDirectories(string directoryPath)
{
	if (!initialized)
		throw runtime_error("int PlatformStorage::GetDirectories() : Not initialized.");

	vector<string>* directoryPaths = new vector<string>();

	path usablePath = GetUsableDirectoryPathFor(directoryPath);
	string usablePathInString = usablePath.string();

	LOG(LogLevel::Debug) << "PlatformStorage::GetDirectories : get [" << usablePathInString << "] for child paths.";

	/* 這邊把子資料夾找出來，directory_iterator只會搜尋下一層資料夾，不會循環搜尋到最底部 */
	for (auto& p : directory_iterator(usablePath)) {

		/* 代表是資料夾不是檔案 */
		if (is_directory(p.path())) {

			string childDirectory = p.path().string();

			// 如果現在的原始路徑是"Build/heresy"，資料夾是"Build/heresy/aout"
			// "Build/heresy"的長度是12，"Build/heresy/aout"的長度是17
			// substr的第一個字是編號1，所以第12個字就是y，從y之後的/開始才是擷取出來的字串，字串長度5個字，就會擷取出"/aout"
			// 如果我想要把第一個/拿掉，就要從第13個字之後擷取，也就是/之後的a開始，擷取4個字，才會擷取出"aout"
			directoryPaths->push_back(usablePathInString.substr(usablePathInString.length() + 1, usablePathInString.length() - childDirectory.length() - 1));

		}
	}
	return directoryPaths;
}

fstream* PlatformStorage::GetStream(string filePath)
{
	if (!initialized)
		throw runtime_error("int PlatformStorage::GetStream() : Not initialized.");


	// 如果沒先check exist會有error
	return new fstream(GetUsableFilePathFor(filePath));
}

fstream * PlatformStorage::GetStream(string filePath, bool volitile, bool binary)
{

	if (binary) {
		return new fstream(filePath, ios::binary|ios::in|ios::out);
	}

	return new fstream(filePath);;
}

string PlatformStorage::locateBasePath()
{
	//暫時先這樣，之後再改
	return string(".");
}

Storage * PlatformStorage::clone(string newSubDirectory)
{
	if (!initialized)
		throw runtime_error("int PlatformStorage::clone() : Not initialized.");

	PlatformStorage* cloned = nullptr;

	if (subDirectory == "") {
		if (newSubDirectory == "")
			cloned = new PlatformStorage(baseName);
		else
			cloned = new PlatformStorage(baseName, newSubDirectory);
	}
	else {
		if (newSubDirectory == "")
			cloned = new PlatformStorage(baseName, subDirectory);
		else
			cloned = new PlatformStorage(baseName, subDirectory + "/"s + newSubDirectory);
	}

	cloned->Initialize();

	return cloned;
}
