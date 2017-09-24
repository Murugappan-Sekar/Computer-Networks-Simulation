//Stop Wait Receiver

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>

char recdata[4][100];
int main(){
	socklen_t len;
	struct sockaddr_in servaddr,cliaddr;
	int sockfd,confd,ptr=0,parity=0,ack,temp,i,j;
	char buff[1024];
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
	printf("\n");
	while(ptr<4){
		read(confd,buff,sizeof(buff));
		printf("\nData received for frame %d : %s\n",ptr%2,buff);
		parity=0;
		for(int i=0;i<4;i++){
			if(buff[i]=='1')
				parity=1-parity;
		}
		strcpy(recdata[ptr],buff);
		if(((char)parity+'0')==buff[5]){
			printf("Data received has no one bit error(maybe correct)\n");
			printf("Do you want to send ack?(1 yes,else no) : ");
			strcpy(buff,"ack");
			temp=1;
		}else{
			printf("Data received has error\n");
			printf("Do you want to send nack?(1 yes,else no) : ");
			strcpy(buff,"nack");
			temp=0;
		}
		scanf("%d",&ack);
		if(ack!=1)
			strcpy(buff,"tle");
		else
			ptr+=temp;
		write(confd,buff,sizeof(buff));
		printf("\n");
	}
	printf("\nData Recived : ");
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			printf("%c ",recdata[i][j] );
	printf("\n");
	close(confd);
	close(sockfd);
	return 0;
}