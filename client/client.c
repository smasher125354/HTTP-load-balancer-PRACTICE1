#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFF 1024

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc , char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
    char newfile[50];
    char buffer[MAX_BUFF];
    if(argc<3)
    {
    	error("uncomplete command line arguments");
    	exit(0);
	}
	portno=atoi(argv[2]);
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0) 
		error("unable to open the socket");
	server=gethostbyname(argv[1]);
	if (server==NULL)
	{
		error("no such server");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
       {
        error("ERROR connecting");
    	}
    printf("Please enter the filename: ");
    
    bzero(buffer,256);
    
    scanf("%s",&buffer);

    strcpy(newfile,buffer);
    
    n = write(sockfd,buffer,strlen(buffer));
    
    if (n < 0)
         error("ERROR writing to socket");
    
    bzero(buffer,256);
    FILE *fp;
    int recv=0;
    fp=fopen(newfile,"a");
    while(1)
    {
    recv=read(sockfd,buffer,256);
    if(recv<=0)
    break;
    fwrite(buffer,1,recv,fp);
    }  
    close(sockfd);
    return 0;
}
