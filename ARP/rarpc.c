//RARP client

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
int main(){
	int sockfd;
	struct sockaddr_in servaddr;
	char buffer[1024];
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		perror("Socker error");
		return 0;
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(5000);
	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	printf("Enter MAC : ");
	gets(buffer);
	write(sockfd,buffer,sizeof(buffer));
	while(1){
		read(sockfd,buffer,sizeof(buffer));
		if(strcmp(buffer,"bye")==0){
			printf("Server terminated the connection\n");
			break;
		}
		printf("IP is  : %s\n\n",buffer);
		printf("Enter MAC(bye to exit) : ");
		gets(buffer);
		if(strcmp(buffer,"bye")==0){
			write(sockfd,buffer,sizeof(buffer));
			printf("You terminated the connection\n");
			break;
		}
		write(sockfd,buffer,sizeof(buffer));
	
	}
	close(sockfd);
	return 0;
}