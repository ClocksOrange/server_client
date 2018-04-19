#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ctype.h>
#include <pthread.h>
#include <string.h>
#include "wrap.h"

#define SERV_PORT 7777

struct c_info
{
	int cfd,i;
	struct sockaddr_in c_addr;
};
void *do_work(void *arg)
{

	struct c_info *ts = (struct c_info *) arg;
	char clie_IP[BUFSIZ],buf[BUFSIZ];;
	int n,i;
//	printf("cfd = ----%d\n", arg->cfd);
	 
 	

    	while (1) {
        	n = Read(ts->cfd, buf, sizeof(buf));
		if(n==0){
			printf("the client %d closed ..\n",ts->cfd);
			break;		
		}

		printf("received client IP: %s  port:%d----->:\n", 
          	  inet_ntop(AF_INET, &(*ts).c_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
           	 ntohs(ts->c_addr.sin_port));
		//fputs(buf,stdout);
		Write(STDOUT_FILENO,buf,n);
	

       		 for (i = 0; i < n; i++)
         	   buf[i] = toupper(buf[i]);
       		 Write(ts->cfd, buf, n);
 	}
	Close(ts->cfd);
	return (void*) 0;
}

int main(void)
{
    int lfd, cfd;
	struct c_info ts[128];
	memset(ts,0,sizeof(ts));
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    
    int  i=0;
	pthread_t tid;

    lfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET, SERV_IP,&serv_addr.sin_addr.s_addr);

    Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    Listen(lfd, 128);
	
    printf("Accepting client connect ...\n ");
	
	while(1)
	{
		cfd = Accept(lfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
	    
		ts[i].cfd=cfd;
		ts[i].c_addr=clie_addr;
		ts[i].i=i;
	
		pthread_create(&tid,NULL,do_work,(void *)&ts[i]);
		//pthread_detach(tid);
		i++;
 	
  	  }	

    return 0;
}
