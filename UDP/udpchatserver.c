//UDP chat server

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>
int main(){
	socklen_t len;
	struct sockaddr_in servaddr,cliaddr;
	int sockfd;
	char buffer[1024];
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
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
	len=sizeof(cliaddr);
	while(1){
		recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)(&cliaddr),&len);
		if(strcmp(buffer,"bye")==0){
			printf("Client terminated the connection\n");
			break;
		}
		printf("Client : %s\n",buffer);
		printf("Enter message : ");
		gets(buffer);
		if(strcmp(buffer,"bye")==0){
			sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)(&cliaddr),len);
			printf("You terminated the connection\n");
			break;
		}
		sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)(&cliaddr),len);
	}
	close(sockfd);
	return 0;
}

