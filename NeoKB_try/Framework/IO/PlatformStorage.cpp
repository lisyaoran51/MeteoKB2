#include "PlatformStorage.h"

#include <experimental/filesystem>
#include <stdexcept>
#include "../../Util/Log.h"

/*
 * filesystem�Ϊk (��boost�@��)
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

	/* �o���l��Ƨ���X�ӡAdirectory_iterator�u�|�j�M�U�@�h��Ƨ��A���|�`���j�M��̩��� */
	for (auto& p : directory_iterator(usablePath)) {


		/* �N��O��Ƨ����O�ɮ� */
		if (is_directory(p.path())) {

			string childDirectory = p.path().string();

			LOG(LogLevel::Debug) << "PlatformStorage::GetDirectories : find subdirectory path [" << childDirectory << "].";

			// �p�G�{�b����l���|�O"Build/heresy"�A��Ƨ��O"Build/heresy/aout"
			// "Build/heresy"�����׬O12�A"Build/heresy/aout"�����׬O17
			// substr���Ĥ@�Ӧr�O�s��1�A�ҥH��12�Ӧr�N�Oy�A�qy���᪺/�}�l�~�O�^���X�Ӫ��r��A�r�����5�Ӧr�A�N�|�^���X"/aout"
			// �p�G�ڷQ�n��Ĥ@��/�����A�N�n�q��13�Ӧr�����^���A�]�N�O/���᪺a�}�l�A�^��4�Ӧr�A�~�|�^���X"aout"
			//directoryPaths->push_back(childDirectory.substr(usablePathInString.length(), usablePathInString.length() - childDirectory.length()));

			directoryPaths->push_back(childDirectory);

		}
	}

	return directoryPaths;
}

fstream* PlatformStorage::GetStream(string filePath)
{
	if (!initialized)
		throw runtime_error("int PlatformStorage::GetStream() : Not initialized.");

	LOG(LogLevel::Debug) << "PlatformStorage::GetStream() : get file [" << GetUsableFilePathFor(filePath) << "].";

	if (!Exist(filePath)) {
		LOG(LogLevel::Debug) << "PlatformStorage::GetStream() : file [" << GetUsableFilePathFor(filePath) << "] not exist.";
		return nullptr;
	}

	// �p�G�S��check exist�|��error
	return new fstream(GetUsableFilePathFor(filePath));
}

fstream * PlatformStorage::GetStream(string filePath, bool volitile, bool binary)
{

	if (binary) {
		return new fstream(locateWriteBasePath() + "/" + filePath, ios::binary|ios::in|ios::out);
	}

	return new fstream(locateWriteBasePath() + "/" + filePath);;
}

fstream * PlatformStorage::GetStream(string filePath, FileAccess fileAccess, FileMode fileMode, bool binary)
{
	/* ���`�إ��ɮת��ܴN�Owrite�Mcreate */
	if (fileAccess == FileAccess::Write && fileMode == FileMode::Create) {

		// TODO: �ˬd���S���n���Ƨ��A�����ܴN������Ƨ��A���ɮ�

		LOG(LogLevel::Debug) << "PlatformStorage::GetStream : get [" << locateWriteBasePath() + string("/") + filePath << "] file stream.";

		if (binary) {
			return new fstream(locateWriteBasePath() + "/" + filePath, ios::binary | ios::out | ios::trunc);
		}
		else {
			return new fstream(locateWriteBasePath() + "/" + filePath, ios::out | ios::trunc);
		}
	}

	return nullptr;
}

string PlatformStorage::locateBasePath()
{
	//�Ȯɥ��o�ˡA����A��
	return string(".");
}

string PlatformStorage::locateWriteBasePath()
{
	return string("/home/pi/temp");
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
