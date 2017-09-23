//UDP echo client

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>
int main(){
	socklen_t len;
	char buffer[1024];
	struct sockaddr_in servaddr;
	int sockfd;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0){
		printf("Error creating socket\n");
		return 0;
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(5000);
	printf("Enter a message : ");
	gets(buffer);
	len=sizeof(servaddr);
	sendto(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)(&servaddr),len);
	recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr *)(&servaddr),&len);
	printf("Echo message from server : %s\n",buffer);
	close(sockfd);
	return 0;
}