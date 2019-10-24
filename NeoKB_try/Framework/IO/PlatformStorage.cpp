#include "PlatformStorage.h"

#include <experimental/filesystem>
#include <stdexcept>



using namespace Framework::IO;
using namespace std::literals::string_literals;
using namespace std::experimental::filesystem;



PlatformStorage::PlatformStorage(string bName): Storage(bName)
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
	return nullptr;
}

ifstream PlatformStorage::GetStream(string filePath)
{
	return ifstream();
}

string PlatformStorage::locateBasePath()
{
	return string(".");
}

Storage * PlatformStorage::clone(string subDirectory)
{
	return nullptr;
}
