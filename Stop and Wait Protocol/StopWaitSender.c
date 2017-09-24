//Stop Wait Sender

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>

int data[16],parity[4];

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
	printf("Enter a 16 bit binary data : ");
	for(i=0;i<16;i++){
		scanf("%d",&data[i]);
		if(data[i]){
			parity[i/4]=1-parity[i/4];
		}
	}
	printf("\nEntered Frames are : \n\n");
	for(i=0;i<4;i++){
		for(j=i*4;j<(i+1)*4;j++)
			printf("%d",data[j]);
		printf("-%d\n",parity[i]);
	}
	printf("\n");
	while(ptr<4){
		printf("\nCurrent frame %d - ",ptr%2);
		for(i=0;i<4;i++)
			buff[i]=data[ptr*4+i]+'0';
		buff[4]='-';
		buff[5]=parity[ptr]+'0';
		buff[6]='\0';
		printf("%s\n",buff);
		printf("Do you want to induce error?(-1 for no,1 to 5 for error position) : ");
		scanf("%d",&pos);
		if(pos<1 || pos>5)
			pos=-1;
		if(pos!=-1 && pos!=5)
			pos--;
		if(pos!=-1)
			buff[pos]='1'-buff[pos]+'0';
		printf("Sendind frame : %s \n",buff);
		write(sockfd,buff,sizeof(buff));
		read(sockfd,buff,sizeof(buff));
		printf("\n");
		if(strcmp(buff,"tle")==0)
			printf("No acknowledment received.\nTrying again\n");
		else if(strcmp(buff,"nack")==0)
			printf("Negative acknowledment received.\nTrying again\n");
		else{
			printf("Acknowledment received.\n");
			ptr++;
		}
	}
	printf("All data sent\n");
	close(sockfd);
	return 0;
}