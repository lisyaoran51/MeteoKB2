#include "PlatformStorage.h"

#include <experimental/filesystem>
#include <stdexcept>

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



PlatformStorage::PlatformStorage(string bName, string sDirectory): Storage(bName, sDirectory)
{
	

}

bool PlatformStorage::Exist(string filePath)
{
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
	vector<string>* directoryPaths = new vector<string>();

	path usablePath = GetUsableDirectoryPathFor(directoryPath);
	string usablePathInString = usablePath.string();

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

ifstream* PlatformStorage::GetStream(string filePath)
{
	// 如果沒先check exist會有error
	return new ifstream(GetUsableFilePathFor(filePath));
}

string PlatformStorage::locateBasePath()
{
	//暫時先這樣，之後再改
	return string(".");
}

Storage * PlatformStorage::clone(string newSubDirectory)
{
	if (subDirectory == "") {
		if (newSubDirectory == "")
			return new PlatformStorage(baseName);
		else
			return new PlatformStorage(baseName, newSubDirectory);
	}
	else {
		if (newSubDirectory == "")
			return new PlatformStorage(baseName, subDirectory);
		else
			return new PlatformStorage(baseName, subDirectory + "/"s + newSubDirectory);
	}

	return nullptr;
}
