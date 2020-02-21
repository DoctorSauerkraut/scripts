#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char*argv[]) {
	if(getuid() !=0) {
		printf("Please run as root");
		return -1;
	}
	system("./0sdjqf &");
	return 0; 

}
