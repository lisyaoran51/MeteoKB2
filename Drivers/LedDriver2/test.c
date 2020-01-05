#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>
#include <signal.h>



 
int main(int argc,char *argv[]){

	int res = 0;


    char buff[96]={0xff};
	
	int i;
	for(i = 0; i < 96; i++){
		buff[i] = 0x0;
	}
	
	buff[1] = 0x34;
	buff[6] = 0xf0;
	buff[24] = 0x73;

	printf("==========================\n");
	printf("START to write FILE\n");
	// non-blocking io read:
	int fd = open("/dev/myBR_file", O_WRONLY /*| O_NONBLOCK */);
	if(fd < 0){
		printf("can't open file myBR_file.\n");
	}else{
		res = write(fd,buff,96);
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
