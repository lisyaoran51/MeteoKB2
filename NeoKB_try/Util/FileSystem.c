#include "FileSystem.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>




int GetFiles(const char* path, char*** fileNames) {

	DIR *dir;
	struct dirent *ent;

	int fileCount = 0;

	// �⦳�X���ɮ�
	if ((dir = opendir(path)) != NULL) {
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {
			fileCount++;
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("intGetFiles(char*, char**) : Failed to open directory.");
		return EXIT_FAILURE;
	}

	// new�@�ӤG�L�}�C
	char *pData;
	(*fileNames) = (char**)malloc(fileCount * sizeof(char *));
	pData = (char*)malloc(fileCount*PATH_LENGTH * sizeof(char));
	for (int i = 0; i < fileCount; i++, pData += PATH_LENGTH)
		(*fileNames)[i] = pData;

	int i = 0;

	// Ū�ɦW�i�h
	dir = opendir(path);
	while ((ent = readdir(dir)) != NULL) {
		strcpy((*fileNames)[i], ent->d_name);
		printf("FileSystem : ------------ Read file: %s ------------\n", (*fileNames)[i]);
		i++;
	}
	closedir(dir);

	return fileCount;

}