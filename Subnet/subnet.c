//subnet program

#include<stdio.h>
#include<string.h>
#define len 32
void ip2bin(int arr[],char s[]){
	int num[4]={0,0,0,0};
	int cur=0,ptr=0;
	while(ptr<strlen(s)){
		if(s[ptr]=='.'){
			ptr++;
			cur++;
		}else{
			num[cur]=num[cur]*10+(s[ptr]-'0');
			ptr++;
		}
	}
	cur=3;
	ptr=len-1;
	while(ptr>=0){
		arr[ptr]=num[cur]%2;
		num[cur]/=2;
		if(ptr%8==0)
			cur--;
		ptr--;
	}
}

void num2bin(int arr[],long long num){
	for(int i=(len-1);i>=0;i--){
		arr[i]=num%2;
		num/=2;
	}
}


void bin2ip(int ip[],int arr[]){
	int cur=3;
	int ptr=31;
	ip[0]=0;
	ip[1]=0;
	ip[2]=0;
	ip[3]=0;
	while(ptr>=0){
		int shift=1<<(7-ptr%8);
		if(arr[ptr]==1)
			ip[cur]+=shift;
		if(ptr%8==0)
			cur--;
		ptr--;

	}
}

long long bin2val(int arr[]){
	int shift=0;
	long long ans=0;
	for(int i=31;i>=0;i--){
		if(arr[i]=='.')
			continue;
		long long val=1LL<<shift;
		if(arr[i]==1)
			ans+=val;
		shift++;
	}
	return ans;
}

void dispip(int ip[]){
	for(int i=0;i<4;i++){
		printf("%d",ip[i]);
		if(i!=3)
			printf(".");
		
	}
}
void main(){
	printf("enter the number of subnets(powers of 2) to be formed : ");
	int n,arr[32],ip[4];
	scanf("%d",&n);
	long long st[n],en[n];
	long long val=1LL<<32;
	val/=n;
	st[0]=0;
	en[0]=val-1;
	for(int i=1;i<n;i++){
		st[i]=en[i-1]+1;
		en[i]=st[i]+val-1;
	}
	printf("subnets are as follows\n");
	for(int i=0;i<n;i++){
		num2bin(arr,st[i]);
		bin2ip(ip,arr);
		printf("subnet %d : ",i);
		dispip(ip);
		printf(" to ");
		num2bin(arr,en[i]);
		bin2ip(ip,arr);
		dispip(ip);
		printf("\n");
	}
	printf("\nuse find to find the subnet number corresponding to the given ip,use details command to get broadcast id of given subnet(0 indexed), and exit to exit\n");
	char cmd[1024];
	while(1){
		scanf("%s",cmd);
		if(strcmp(cmd,"exit")==0)
			break;
		else if(strcmp(cmd,"find")==0){
			char buffer[1024];
			scanf("%s",buffer);
			ip2bin(arr,buffer);
			val=bin2val(arr);
			if(val>en[n-1])
				printf("INVALID IP ADDRESS\n");
			else{
				for(int i=0;i<n;i++){
					if(val>=st[i] && val<=en[i]){
						printf("IT BELONGS TO SUBNET %d\n",i);
						break;
					}
				}
			}
		}else if(strcmp(cmd,"details")==0){
			int ch;
			scanf("%d",&ch);
			if(ch>=n){
				printf("INVALID SUBNET NUMBER\n");
			}else{
				num2bin(arr,st[ch]);
				bin2ip(ip,arr);
				printf("subnet %d : ",ch);
				dispip(ip);
				printf(" to ");
				num2bin(arr,en[ch]);
				bin2ip(ip,arr);
				dispip(ip);
				printf(" (Broadcast ID)");
				printf("\n");
			}
		}else
			printf("INVALID COMMAND\n");

	}
}