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
	
	int variant = 0;
	
	if(argc > 1)
		variant = atoi(argv[1]);
	
	unsigned char** matrix = draw(variant);
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
				
		}
	}
	
	for(i = 0; i < 16; i++){
		int j;
		for(j = 0; j < 6; j++)
			printf("%08x ",lightMatrixMessage[i*6+j]);
		printf("\n");
	}

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



	if(variant == 10){
		int k;
		for(k = 0; k < 5; k++){
			matrix = draw(k);
			
			for (i = 0; i < width; i++) {
				int j;
				for (j = 0; j < height; j++) {

					if (matrix[i][height - 1 - j] > 0)
						lightMatrixMessage[j * 6 + i / 8] |= (0x01 << (i % 8));
						
				}
			}
			
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
			
			sleep(5);
			
			for(i = 0; i < 96; i++)
				lightMatrixMessage[i] = 0;
			res = write(fd,lightMatrixMessage,96);
			close(fd);
			if(k == 4)
				k = -1;
		
		}
	}

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
	
	switch(variant){
		case 0:
			matrix[7][0] = 1;
			matrix[14][5] = 1;
			matrix[19][10] = 1;
			matrix[31][1] = 1;
			matrix[33][3] = 1;
			matrix[35][5] = 1;
			matrix[36][7] = 1;
			matrix[38][9] = 1;
			matrix[43][0] = 1;
		
		break;
		case 1:
			matrix[12][0] = 1;
			matrix[14][14] = 1;
			matrix[19][7] = 1;
			matrix[24][2] = 1;
			matrix[24][8] = 1;
			matrix[26][0] = 1;
			matrix[26][6] = 1;
			matrix[31][14] = 1;
		
		break;
		case 2:
			matrix[10][6] = 1;
			matrix[12][15] = 1;
			matrix[17][8] = 1;
			matrix[26][15] = 1;
			matrix[31][11] = 1;
			matrix[33][7] = 1;
			matrix[36][3] = 1;
			matrix[38][0] = 1;
		
		break;
		case 3:
			matrix[11][0] = 1;
			matrix[16][6] = 1;
			matrix[19][12] = 1;
			matrix[26][12] = 1;
			matrix[28][6] = 1;
			matrix[29][0] = 1;
			matrix[38][12] = 1;
			matrix[40][6] = 1;
			matrix[41][0] = 1;
		
		break;
		case 4:
			matrix[7][6] = 1;
			matrix[12][12] = 1;
			matrix[17][12] = 1;
			matrix[24][12] = 1;
			matrix[26][2] = 1;
			matrix[30][7] = 1;
			matrix[30][15] = 1;
			matrix[31][12] = 1;
			matrix[33][2] = 1;
		
		break;
		case 5:
		
		break;
		case 6:
		
		break;
		
	}
	
	
	
	return matrix;
}
