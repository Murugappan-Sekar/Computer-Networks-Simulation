//UDP chat client

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
int main(){
	int sockfd;
	struct sockaddr_in servaddr;
	char buffer[1024];
	socklen_t len;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0){
		perror("Socker error");
		return 0;
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(5000);
	len=sizeof(servaddr);
	printf("Enter message : ");
	gets(buffer);
	sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)(&servaddr),len);
	while(1){
		recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)(&servaddr),&len);
		if(strcmp(buffer,"bye")==0){
			printf("Server terminated the connection\n");
			break;
		}
		printf("Server : %s\n",buffer);
		printf("Enter message : ");
		gets(buffer);
		if(strcmp(buffer,"bye")==0){
			sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)(&servaddr),len);
			printf("You terminated the connection\n");
			break;
		}
		sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)(&servaddr),len);
	}
	close(sockfd);
	return 0;
}