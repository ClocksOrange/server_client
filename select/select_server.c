#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include "wrap.h"
#include<unistd.h>
#include <ctype.h>

#define MAXLINE 80
#define SERV_PORT 6666
int main()
{
	int i,maxi,maxfd,listenfd,connfd,sockfd;
	int nread,client[FD_SETSIZE];
	ssize_t n;
	fd_set reset,allset;
	char str[INET_ADDRSTRLEN],buf[BUFSIZ];
	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	

    	int opt = 1;
   	 setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	socklen_t cliaddr_len;
	struct sockaddr_in cliaddr,servaddr;

	bzero(&servaddr,sizeof(servaddr));
	//ser	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port=htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	Bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	
	Listen(listenfd,20);
	
	maxfd =listenfd;
	maxi =-1;
	
	for(i=0;i<FD_SETSIZE;i++)
		client[i]=-1;
	
	FD_ZERO(&allset);
	FD_SET(listenfd,&allset);
	
	while(1)
	{
		reset = allset;
		nread = select(maxfd+1,&reset,NULL,NULL,NULL);
		
		if(nread<0)
		{
			perr_exit("select error");
		}
		if(FD_ISSET(listenfd,&reset))
		{
			
			cliaddr_len = sizeof(cliaddr);
			connfd = Accept(listenfd,(struct sockaddr *)&cliaddr,&cliaddr_len);
			printf("received from %s at port %d\n",inet_ntop(AF_INET,&cliaddr.sin_addr,str,sizeof(str)),ntohs(cliaddr.sin_port));
			for(i=0;i<FD_SETSIZE;i++){
				if(client[i]<0)
				{ client[i]=connfd;break;	}
			}
			//printf("received from2\n");
			FD_SET(connfd,&allset);
			if(maxfd<connfd)
				maxfd=connfd;
			if(maxi<i) maxi=i;
			
			if(--nread==0)	continue;
		}
		for(i=0;i<=maxi;i++)
		{
			if((sockfd=client[i])<0){
				continue;
			}
			if(FD_ISSET(client[i],&reset)){
				
				if((n = Read(sockfd,buf,sizeof(buf)))==0)
				{
					Close(sockfd);
					FD_CLR(sockfd,&allset);
					client[i]=-1;
				}
				else if(n>0)
				{
					int j;
					for(j=0;j<n;j++)
					
						buf[j]= toupper(buf[j]);

					Write(sockfd,buf,n);
					Write(STDOUT_FILENO,buf,n);
					
				}
				if(--nread ==0)
				{
					break;
				}
			

			}
			

		}
	}
	close(listenfd);
	return 0;
}
