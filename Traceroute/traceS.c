//Traceroute server

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>
#define maxn 50
int parent[maxn],depth[maxn],n,m;
int adjmatrix[maxn][maxn];
void clearold(){
	for(int i=0;i<maxn;i++){
		parent[i]=0;
		depth[i]=0;
	}
}

void getip(){
	printf("enter number of nodes and edges : ");
	scanf("%d %d",&n,&m);
	int u,v;
	printf("enter edges : \n");
	for(int i=0;i<m;i++){
		scanf("%d %d",&u,&v);
		adjmatrix[u][v]=1;
		adjmatrix[v][u]=1;
	}
	printf("Data entered\n");

}
void dfs(int cur){
	for(int i=0;i<maxn;i++){
		if(adjmatrix[cur][i]==1 && parent[i]==0){
			parent[i]=cur;
			depth[i]=depth[cur]+1;
			dfs(i);
		}
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

	getip();
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
		int u=0,v=0,i;
		for(i=0;buffer[i]!=' ';i++)
			u=u*10+(buffer[i]-'0');
		i++;
		for(;buffer[i]!='\0';i++)
			v=v*10+(buffer[i]-'0');
		printf("Client asked for path between: %d - %d \n",u,v);
		clearold();
		parent[v]=-1;
		dfs(v);
		if(parent[u]==0)
			strcpy(buffer,"NO PATH EXISTS");
		else{
			for(int i=0;i<1024;i++)
			buffer[i]=' ';
			int cur=u;
			int ptr=0;
			while(1){
				buffer[ptr]=cur+'0';
				ptr+=2;
				if(cur==v)
					break;
				cur=parent[cur];
			}
			buffer[ptr-1]='\0';
		}
		write(confd,buffer,sizeof(buffer));
	}
	close(confd);
	close(sockfd);
	return 0;
}