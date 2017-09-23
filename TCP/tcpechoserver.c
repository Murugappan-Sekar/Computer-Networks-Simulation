
//TCP echo server

#include<sys/types.h>
	//contains definitions which are used by the following header files

#include<sys/socket.h>
	/*
	socket.h(internet protocol family) contains socklen_t,SOCK_STREAM,AF_INET
	*/

#include<netinet/in.h>
	/*
		contains structure for handling internet address(sockaddr_in),htons
	*/
#include<stdio.h>
	//std definitions and perror
#include<string.h>
	//contains bzero function
// bind and listen are half contained in types and socket.h

int main(int argc,char **argv)
{
	socklen_t len;
		//size type for sockaddr(__in) structure
		//we dont use the type int because int size is define by the compiler(32 or 64)
	
	int sockfd,connfd,n;
		/*
		descriptors(unique id) of socket,connection and read operation respectively
		value<0 indicates error
		*/
	struct sockaddr_in servaddr,cliaddr;
		//structure for handling internet address
	char buff[1024];
		//temporary storage for the data to be handled by the compiler
	sockfd=socket(AF_INET,SOCK_STREAM,0);
		/*
			AF_INET - ipv4(internetwork)
			SOCK_STREAM - TCP Stream :: numberical value 1
		*/
	if(sockfd < 0)
	{
		//socket id can't be negative
		perror("\nunable to create socket\n\n");
		return 0;
	}
	bzero(&servaddr,sizeof(servaddr));
		//equivalent to memset_0 ,setting all values in the variable as 0
	servaddr.sin_family=AF_INET;
		//description of what kind of network the structure is going to handle
		//(server internet family) type is short
	servaddr.sin_addr.s_addr=INADDR_ANY;
		//sin_addr is a structure with one state variable s_addr(server address)
	servaddr.sin_port=htons(5000);
		//server port number in network format(might be little endian)
		//htons - host to network convert type of short
	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
		//bind physical socket to socket handler structure
		
	listen(sockfd,0);
		//sock decriptor and backlog waiting connection size
	len=sizeof(cliaddr);
	connfd=accept(sockfd,(struct sockaddr *)&cliaddr,&len);
		//accept client connection through the socket created
		//accept is defined in general for all sockets(internet and intranet),so internet socket structure must be converted to general socket structure
	n=read(connfd,buff,sizeof(buff));
		//normal read call(source,dest,maxsize of dest)
	printf("Message Received :%s",buff);
	write(connfd,buff,sizeof(buff));
		//normal write call(dest,source data,size of sourcedata)
	close(connfd);
	close(sockfd);
	printf("\n");
	return 0;
}
	
