//Ping server

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>
int main(){
	socklen_t len;
	struct sockaddr_in servaddr,cliaddr;
	int n,sockfd,confd;
	char buffer[1024];
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		perror("Error in socket creation\n");
		return 0;
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(5000);
	if(bind(sockfd,(struct sockaddr*)(&servaddr),sizeof(servaddr))<0){
		perror("binding error\n");
		return 0;
	}
	listen(sockfd,0);
	len=sizeof(cliaddr);
	confd=accept(sockfd,(struct sockaddr*)(&cliaddr),&len);
	if(confd<0){
		perror("connection error\n");
		return 0;
	}
	int ptr=0;
	while(ptr<10){
		ptr++;
		read(confd,buffer,sizeof(buffer));
		write(confd,buffer,sizeof(buffer));
	}
	close(confd);
	close(sockfd);
	return 0;
}