//UDP file server

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
int main(int argc,char **argv)
{
	socklen_t len;
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;
	char buff[1024];
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd < 0) {
		perror("\nunable to create socket\n\n");
		return 0;
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(5000);
	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	len=sizeof(cliaddr);
	recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)(&cliaddr),&len);
	if(strcmp(buff,"error")==0){
		printf("Some error in client side\n");
		close(sockfd);
		return 0;
	}
	printf("Name of the file received :%s\n",buff);
	FILE* fp=fopen(buff,"w");
	if(fp==NULL){
		printf("File couldn't be created\n");
		strcpy(buff,"error");
		sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr*)(&cliaddr),len);	
	}else{
		strcpy(buff,"ok");
		sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr*)(&cliaddr),len);
		while(1){
			recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)(&cliaddr),&len);
			if(strcmp(buff,"close_file")==0){
				break;
			}else{
				fputs(buff,fp);
			}
		}
		fclose(fp);
		printf("File Transfer Successful\n");
		strcpy(buff,"ok");
		sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr*)(&cliaddr),len);
	}
	close(sockfd);
	return 0;
}