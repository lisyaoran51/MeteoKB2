#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>
#include <signal.h>

// gcc ForShooting.c -o ForShooting


unsigned char** draw(int variant);
 
int main(int argc,char *argv[]){

	int res = 0;


    char buff[96]={0xff};
	
	int i;
	for(i = 0; i < 96; i++){
		buff[i] = 0x0;
	}
	
	unsigned char** matrix = draw(0);
	char lightMatrixMessage[96];
	
	
	
	int width = 48;
	int height = 16;
	for (i = 0; i < 96; i++) {
		lightMatrixMessage[i] = 0x0;
	}

	for (i = 0; i < width; i++) {
		int j;
		for (j = 0; j < height; j++) {

			if (matrix[i][height - 1 - j] > 0)
				lightMatrixMessage[j * 6 + i / 8] |= (0x01 << (i % 8));
				printf(" %d",matrix[i][height - 1 - j]);
		}
		printf("\n");
	}
	
	buff[1] = 0x34;
	buff[6] = 0xf0;
	buff[24] = 0x73;

	printf("==========================\n");
	printf("START to write FILE\n");
	// non-blocking io read:
	int fd = open("/dev/meteo_lightboard_v1", O_WRONLY /*| O_NONBLOCK */);
	if(fd < 0){
		printf("can't open file meteo_lightboard_v1.\n");
	}else{
		res = write(fd,lightMatrixMessage,96);
		if(res < 0){
		        perror("test:");
		}else{
			printf("### write function return: %d\n",res);
		}
	}
	close(fd);
	printf("==========================\n");



	return 0;
}

unsigned char** draw(int variant){
	
	unsigned char** matrix = (unsigned char**)malloc(48 * sizeof(unsigned char*));
	
	int i;
	for(i = 0; i < 48; i++){
		matrix[i] = (unsigned char*)malloc(16 * sizeof(unsigned char));
	}
	
	for(i = 0; i < 48; i++){
		int j;
		for(j = 0; j < 16; j++)
			matrix[i][j] = 0;
	}
	matrix[12][3] = 1;
	matrix[10][6] = 1;
	
	return matrix;
}
