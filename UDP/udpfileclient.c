//UDP file client

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
	socklen_t len;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0){
		perror("\nsocket error\n");
		return 0;
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(5000);
	len=sizeof(servaddr);
	printf("Enter a file name : ");
	gets(str);
	FILE* fp=fopen(str,"r");
	if(fp==NULL){
		strcpy(buff,"error");
		sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&servaddr,len);
		printf("File couldn't be opened\n");
		close(sockfd);
		return 0;
	}else{
		printf("Enter name to save the file : ");
		gets(str);
		sendto(sockfd,str,sizeof(str),0,(struct sockaddr*)&servaddr,len);
		recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&servaddr,&len);
		if(strcmp(buff,"error")==0){
			printf("Some error in server side\n");
			close(sockfd);
			close(fp);
			return 0;
		}
		while(fgets(str, 500, fp)!=NULL)
			sendto(sockfd,str,sizeof(str),0,(struct sockaddr*)&servaddr,len);
		strcpy(buff,"close_file");
		sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&servaddr,len);
	}
	recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&servaddr,&len);
	if(strcmp(buff,"error")==0)
		printf("Some error in server side\n");
	else
		printf("file transfer successful\n");
	close(sockfd);
	return 0;
}