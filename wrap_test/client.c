#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include<ctype.h>
#include "wrap.h"
#define SERV_IP "192.168.249.129"
#define SERV_PORT 6666
int main()
{
	int cfd;
	char buf[BUFSIZ];
	 int n;
	struct sockaddr_in serv_addr;
	//socklen_t serv_addr_len;	
	cfd= Socket(AF_INET,SOCK_STREAM,0);
	memset(&serv_addr,0,sizeof(serv_addr));
	//bind();
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(SERV_PORT);
	//serv_addr.sin_addr.s_addr=htonl( );
	inet_pton(AF_INET,SERV_IP,&serv_addr.sin_addr.s_addr);
	
	Connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	
	while(1){
		fgets(buf,sizeof(buf),stdin);
		write(cfd,buf,strlen(buf));
		n =Read(cfd,buf,sizeof(buf));
		Write(STDOUT_FILENO,buf,n);	
	}
	
	close(cfd);
	return 0;
}
