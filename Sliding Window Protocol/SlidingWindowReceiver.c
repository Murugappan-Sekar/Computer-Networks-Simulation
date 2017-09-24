//Sliding Window Receiver

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>

char recdata[100];
int pos;
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
	pos=0;
	temp=0;
	while(ptr<8){
		temp=-1;
		read(confd,buff,sizeof(buff));
		printf("\nData received for frames: ");
		for(i=ptr;i<8 && i<(ptr+3);i++)
			printf("%d ",i%4);
		printf("\n");
		printf("\nData received : %s",buff);
		printf("\nSend ack number (invalid number means its no ack) : ");
		scanf("%d",&ack);
		for(int i=ptr;i<=8 && i<(ptr+4);i++)
			if(ack==i%4)
				temp=i-ptr;
		if(temp==-1)
			strcpy(buff,"tle");
		else{

			for(i=pos;i<(pos+temp*4);i++)
				recdata[i]=buff[i-pos];
			pos+=temp*4;
			strcpy(buff,"ack");
			buff[3]=ptr+temp+'0';
			buff[4]='\0';
			ptr+=temp;
		}
		write(confd,buff,sizeof(buff));
		printf("\n");
	}
	recdata[32]='\0';
	printf("\nData Recived : %s\n",recdata);
	close(confd);
	close(sockfd);
	return 0;
}