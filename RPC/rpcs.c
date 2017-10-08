//RPC Server

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
void decoder(char ip[]){
	int num=0;
	int fst=1500,est=1500;
	for(int i=0;i<strlen(ip);i++){
		if(ip[i]=='(')
			fst=i;
		else if(ip[i]==')'){
			if(fst<i)
				est=i;
			else
				est=-1;

		}
	}
	if(est==-1){
		strcpy(ip,"invalid command");
	}else{
		for(int i=fst+1;i<est;i++){
			num=num*10+(ip[i]-'0');
		}
		ip[fst]='\0';
		num=pow(num,2);
		if(strcmp(ip,"square")!=0){
			strcpy(ip,"invalid command");	
		}else

			snprintf(ip,sizeof(ip),"%d",num);
	}
}
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

	
	while(1){
		read(confd,buffer,sizeof(buffer));
		
		if(strcmp(buffer,"bye")==0){
			printf("Client terminated the connection\n");
			break;
		}
		printf("Client Requested : %s\n",buffer);
		decoder(buffer);
		write(confd,buffer,sizeof(buffer));
	}
	close(confd);
	close(sockfd);
	return 0;
}