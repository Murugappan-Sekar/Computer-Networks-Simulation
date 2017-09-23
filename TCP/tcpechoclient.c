//TCP echo client

#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
int main(int argc,char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	char str[1024],buff[1024];
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
		perror("\nsocket error\n");
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(5000);
	//same description as of in server program
	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	//
	printf("Enter a Message:");
	gets(str);
	write(sockfd,str,sizeof(str));
	read(sockfd,buff,sizeof(buff));
	printf("Message Echoed: %s",buff);
	printf("\n");
	close(sockfd);
	return 0;
}

