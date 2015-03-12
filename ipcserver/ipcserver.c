#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_BUFF 1024


void error(char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc ,char *argv[])
{
	int fd;
	FILE *fp;
    char * myfifo = "/tmp/myfifo";
    int clisock,cliport,clilen,newsockfd,n,i;
    struct sockaddr_in serv_addr,cli_addr;
    char buffer[MAX_BUFF];
    
    if(argc<2)
	{
		error("error port no is not given");
		exit(0);
	}
	clisock=socket(AF_INET,SOCK_STREAM,0);
	if(clisock<0)
	{
		error("unable to create socket");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
     
    cliport = atoi(argv[1]);
     
    serv_addr.sin_family = AF_INET;
     
    serv_addr.sin_addr.s_addr = INADDR_ANY;
     
    serv_addr.sin_port = htons(cliport);
     
    if (bind(clisock, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
              error("ERROR on binding");
    
    listen(clisock,5);
     
    clilen = sizeof(cli_addr);
     
	newsockfd = accept(clisock, (struct sockaddr *) &cli_addr, &clilen);
    
    if (newsockfd < 0)
          error("ERROR on accept");
    char buf[MAX_BUFF];
	fd = open(myfifo, O_RDONLY);
    read(fd, buffer, MAX_BUFF);
    if(access(buffer,F_OK)!=-1)
    {
    	fp = fopen(buffer,"r"); // read mode

 	if( fp == NULL )
   	{
      		error("Error wh1ile opening the file.\n");
      		exit(EXIT_FAILURE);
   	}
   	int s=fread(buffer,1,256,fp);
   	while(s>0)
   	{
   		write(newsockfd,buffer,s);
   		s=fread(buffer,1,256,fp);
	}
   	fclose(fp);

    }

    close(fd);
    return 0;
}