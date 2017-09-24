//Sliding Window Sender

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>

int data[32];

int main(){
	int sockfd,i,j,pos,ptr=0;
	struct sockaddr_in servaddr;
	char buff[1024];
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
	printf("Enter a 32 bit binary data : ");
	for(i=0;i<32;i++){
		scanf("%d",&data[i]);
	}
	printf("\nEntered Frames are : \n\n");
	for(i=0;i<8;i++){
		for(j=i*4;j<(i+1)*4;j++)
			printf("%d",data[j]);
		printf("\n");
	}
	printf("\nWindow Size : 3 frames \n" );
	printf("\n");
	while(ptr<8){
		printf("\nCurrent frames : ");
		for(i=ptr;i<8 && i<(ptr+3);i++)
			printf("%d ",i%4);
		printf("\n");
		for(i=ptr;i<8 && i<(ptr+3);i++){
			for(j=0;j<4;j++)
				buff[(i-ptr)*4+j]='0'+data[i*4+j];
		}
		buff[(i-ptr)*4]='\0';
		printf("Sending data : %s \n",buff);
		write(sockfd,buff,sizeof(buff));
		read(sockfd,buff,sizeof(buff));
		printf("\n");
		if(buff[0]=='t')
			printf("No acknowledment received.\nTrying again\n");
		else if(buff[0]=='a'){
			int val=buff[3]-'0';
			printf("Acknowledment %d received.\n",val%4);
			ptr+=(val-ptr);
		}
	}
	printf("All data sent\n");
	close(sockfd);
	return 0;
}