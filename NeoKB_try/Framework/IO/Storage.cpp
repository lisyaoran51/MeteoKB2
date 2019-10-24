#include "Storage.h"

#include <experimental/filesystem>
#include <fstream>


using namespace Framework::IO;
using namespace std::literals::string_literals;
using namespace std::experimental::filesystem;



string Storage::GetUsableDirectoryPathFor(string directoryPath, bool createIfNotExist)
{
	string resolvedPath = basePath + "/"s + baseName;
	if (subDirectory != "")
		resolvedPath += "/"s + subDirectory;
	resolvedPath += "/"s + directoryPath;

	string temp;
	temp = resolvedPath.substr(resolvedPath.length() - 1, 1);
	if (temp != "/"s)
		resolvedPath += "/"s;
	

	path resolved = resolvedPath;

	if (createIfNotExist && is_directory(resolved) == 0)
		create_directories(resolved);


	return resolvedPath;
}

string Storage::GetUsableFilePathFor(string filePath, bool createIfNotExist)
{
	string resolvedPath = basePath + "/"s + baseName;
	if (subDirectory != "")
		resolvedPath += "/"s + subDirectory;
	resolvedPath += "/"s + filePath;

	path resolved = resolvedPath;

	if (createIfNotExist && exists(resolved) == 0) {
		create_directories(resolved.parent_path());
		ofstream ofs(resolved);
		ofs.close();
	}

	return resolvedPath;
}

Storage * Storage::GetStorageForDirectory(string directoryPath)
{
	GetUsableDirectoryPathFor(directoryPath, true);
	if (subDirectory == "") {
		string temp = directoryPath.substr(directoryPath.length() - 1, 1);
		if (temp != "/"s)
			directoryPath += "/"s;
		return clone(directoryPath);

	}
	else {
		string resolvedPath = subDirectory + directoryPath;
		string temp = resolvedPath.substr(resolvedPath.length() - 1, 1);
		if (temp != "/"s)
			resolvedPath += "/"s;
		return clone(resolvedPath);
	}

	return nullptr;
}

Storage::Storage(string bName, string sDirectory)
{
	baseName = bName;
	basePath = locateBasePath();
	subDirectory = sDirectory;
}
