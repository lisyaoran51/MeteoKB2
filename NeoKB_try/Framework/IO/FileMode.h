#ifndef FILE_MODE_H
#define FILE_MODE_H

namespace Framework {
namespace IO {

	enum class FileMode {

		//
		// �K�n:
		//     ���w�@�~�t�����ӫإ߷s���ɮסC �o�ݭn System.Security.Permissions.FileIOPermissionAccess.Write
		//     �v���C �p�G�ɮפw�g�s�b�A System.IO.IOException �Y�^�ҥ~���p�C
		CreateNew,
		//
		// �K�n:
		//     ���w�@�~�t�����ӫإ߷s���ɮסC �p�G�ɮפw�g�s�b�A�h�|�мg�C �o�ݭn System.Security.Permissions.FileIOPermissionAccess.Write
		//     �v���C FileMode.Create �۷��n�D���ɮפ��s�b�A�p�G�ϥ� System.IO.FileMode.CreateNew; �_�h�A�Шϥ� System.IO.FileMode.Truncate�C
		//     �p�G�ɮפw�g�s�b�A���O���ê��ɮסA System.UnauthorizedAccessException �Y�^�ҥ~���p�C
		Create,
		//
		// �K�n:
		//     ���w�@�~�t�����Ӷ}�Ҳ{�����ɮסC �}���ɮת��\����M��ҫ��w���� System.IO.FileAccess �C�|���O�C A System.IO.FileNotFoundException
		//     �p�G�ɮפ��s�b�A�|�Y�^�ҥ~���p�C
		Open,
		//
		// �K�n:
		//     ���w�@�~�t�����Ӷ}���ɮסA�O�_�����ܡA�_�h�A�z���ӫإ߷s���ɮסC �p�G�P�}�Ҫ��ɮ� FileAccess.Read, �ASystem.Security.Permissions.FileIOPermissionAccess.Read
		//     �ݭn���v���C �p�G�ɮצs���O FileAccess.Write, �ASystem.Security.Permissions.FileIOPermissionAccess.Write
		//     �ݭn���v���C �p�G�P�}�Ҫ��ɮ� FileAccess.ReadWrite, �A�o��� System.Security.Permissions.FileIOPermissionAccess.Read
		//     �M System.Security.Permissions.FileIOPermissionAccess.Write ���ݭn�v���C
		OpenOrCreate,
		//
		// �K�n:
		//     ���w�@�~�t�����Ӷ}�Ҳ{�����ɮסC �}���ɮ׮ɡA���ӺI�_�A�]����j�p���s�Ӧ줸�աC �o�ݭn System.Security.Permissions.FileIOPermissionAccess.Write
		//     �v���C ���ձq�ɮ�Ū���H�}�� FileMode.Truncate �y�� System.ArgumentException �ҥ~���p�C
		Truncate,
		//
		// �K�n:
		//     �p�G�s�b�M�M���ɮת������A�H�K�Ϋإ߷s���ɮסA�ж}���ɮסC �o�ݭn System.Security.Permissions.FileIOPermissionAccess.Append
		//     �v���C FileMode.Append �u�Ω�f�t FileAccess.Write�C ���շj�M���ɮ׫h�|�Y�^�������e����m System.IO.IOException
		//     �ҥ~���p�A�H�Υ������Ū���|���ѡA�h�Y�^ System.NotSupportedException �ҥ~���p�C
		Append

	};


}}



#endif