#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>


#define PG_SIZE 1024

char write_buf[PG_SIZE];
char read_buf[PG_SIZE];

int main()
{
	int fd;
	int option;
	int ret;

	printf("*************************************\n");
	printf("******WWWW.EMBETRONICS.COM***********\n");

	fd = open("/dev/etx_device", O_RDWR);
	if (fd < 0) {
		printf("Cannot open device file....fd = %d\n", fd);
		perror("Message from perror = %s\n");
		return 0;
	}

	while(1) {
		printf("***Pls enter your choice*****\n");
		printf("    1) Write            \n");
		printf("    2) Read            \n");
		printf("    3) Exit            \n");
		printf("*************************\n");
		scanf("%d", &option);
		printf("Your Option = %d\n", option);

		switch(option) {
			case 1:
				printf("Enter the string to write to the driver\n");
				scanf("  %[^\t\n]s", write_buf);
				printf("Data Writing ...\n");
				ret = write(fd, write_buf, strlen(write_buf) + 1);
				printf("Done!\n");
				break;

			case 2:
				printf("Data Reading.....\n");
				ret = read(fd, read_buf, 1024);
				if (ret < PG_SIZE) {
					perror("Message from perror %s\n");
					exit(0);
				}
				printf("Done Reading!\n\n");
				printf("Data = %s\n\n", read_buf);
				break;
			case 3:
				close(fd);
				exit(1);
				break;
			default:
				printf("Enter valid option = %d\n", option);
				break;
		}
	
	}
	close(fd);
}
