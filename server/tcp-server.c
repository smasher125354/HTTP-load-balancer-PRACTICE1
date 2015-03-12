#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_BUFF 1024

void error(char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc,char *argv[])
{
	int fd;
    char * myfifo = "/tmp/myfifo";
    int clisock,cliport,clilen,newsockfd,n,i;
    int sockfd, portno, n1;
    struct hostent *server;
	int len;
	char ch, file_name[25];
	FILE *fp;
	struct dirent *pDirent;
        DIR *pDir;
		
	char buffer[MAX_BUFF];
	struct sockaddr_in serv_addr,cli_addr,ipcserv_addr;
	if(argc<4)
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
    
    bzero(buffer,256);
    
    n = read(newsockfd,buffer,255);
    
    if (n < 0) error("ERROR reading from socket");
	    printf("Here is the message: %s\n",buffer);
	    strcpy(file_name,buffer);
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
     else
	{
	portno=atoi(argv[3]);
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0) 
		error("unable to open the socket");
	server=gethostbyname(argv[2]);
	if (server==NULL)
	{
		error("no such server");
		exit(0);
	}
	bzero((char *) &ipcserv_addr, sizeof(ipcserv_addr));
    ipcserv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&ipcserv_addr.sin_addr.s_addr,server->h_length);
    ipcserv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&ipcserv_addr,sizeof(ipcserv_addr)) < 0)
       {
        error("ERROR connecting");
    	}
	mkfifo(myfifo, 0666);
	fd = open(myfifo, O_WRONLY);
    write(fd, buffer, sizeof(buffer));
    close(fd);
	unlink(myfifo);
	FILE *fp;
    int recv=0;
    fp=fopen(buffer,"a");
    while(1)
    {
    	recv=read(sockfd,buffer,256);
    	if(recv<=0)
    	break;
    	fwrite(buffer,1,recv,fp);
    }  
    	fp = fopen(file_name,"r"); // read mode
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
	close(clisock);	   
    return 0;
}  
