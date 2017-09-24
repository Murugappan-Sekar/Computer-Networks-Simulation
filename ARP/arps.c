//ARP Server

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>
char ip[100][1024],mac[100][1024];
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
	printf("Enter number of entries in the ip table : ");
	scanf("%d",&n);
	for(int i=1;i<=n;i++){
		printf("Enter the ip and corresponding mac address : ");
		scanf("%s %s",ip[i],mac[i]);
	}
	confd=accept(sockfd,(struct sockaddr*)(&cliaddr),&len);
	if(confd<0){
		perror("connection error\n");
		return 0;
	}

	
	while(1){
		read(confd,buffer,sizeof(buffer));
		
		if(strcmp(buffer,"bye")==0){
			printf("Client terminated the connection\n");
			break;
		}
		printf("Client Requested MaC for IP: %s\n",buffer);
		int temp=0;
		for(int i=1;i<=n;i++){
			if(strcmp(ip[i],buffer)==0){
				temp=1;
				strcpy(buffer,mac[i]);
			}
		}
		if(temp==0)
			strcpy(buffer,"No corresponding mac found");
		write(confd,buffer,sizeof(buffer));
	}
	close(confd);
	close(sockfd);
	return 0;
}