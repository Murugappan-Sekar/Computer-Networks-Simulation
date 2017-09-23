//TCP file client

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
	printf("Enter a file name : ");
	gets(str);
	FILE* fp=fopen(str,"r");
	if(fp==NULL){
		strcpy(buff,"error");
		write(sockfd,buff,sizeof(buff));
		printf("File couldn't be opened\n");
		close(sockfd);
		return 0;
	}else{
		printf("Enter name to save the file : ");
		gets(str);
		write(sockfd,str,sizeof(str));
		read(sockfd,buff,sizeof(buff));
		if(strcmp(buff,"error")==0){
			printf("Some error in server side\n");
			close(sockfd);
			close(fp);
			return 0;
		}
		while(fgets(str, 500, fp)!=NULL)
			write(sockfd,str,sizeof(str));
		strcpy(buff,"close_file");
		write(sockfd,buff,sizeof(buff));
	}
	read(sockfd,buff,sizeof(buff));
	if(strcmp(buff,"error")==0)
		printf("Some error in server side\n");
	else
		printf("file transfer successful\n");
	close(sockfd);
	return 0;
}

